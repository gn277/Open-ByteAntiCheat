////////////////////////////////////////////////////////////////
//						内存重映射
////////////////////////////////////////////////////////////////
#include "../BAC.h"
#include "../../BAC-Base.h"

#pragma comment(lib,"ntdll.lib")

#define PAGE_SIZE 0x1000
#define SEC_NO_CHANGE 0x00400000

#define NtCurrentProcess()  ((HANDLE)(LONG_PTR)-1)
#define POINTER_IS_ALIGNED(Pointer, Alignment) \
    (((((ULONG_PTR)(Pointer)) & (((Alignment)-1))) == 0) ? TRUE : FALSE)

typedef enum _SECTION_INHERIT
{
    ViewShare = 1,
    ViewUnmap = 2
} SECTION_INHERIT, * PSECTION_INHERIT;
typedef enum _MEMORY_INFORMATION_CLASS
{
    MemoryBasicInformation
} MEMORY_INFORMATION_CLASS, * PMEMORY_INFORMATION_CLASS;
typedef BOOL(NTAPI* REMAP_ROUTINE)(_In_ PVOID p_remap_region);

extern "C" PIMAGE_NT_HEADERS NTAPI RtlImageNtHeader(_In_ PVOID base_address);
extern "C" NTSTATUS NTAPI NtCreateSection(
    _Out_    PHANDLE            SectionHandle,
    _In_     ACCESS_MASK        DesiredAccess,
    _In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
    _In_opt_ PLARGE_INTEGER     MaximumSize,
    _In_     ULONG              SectionPageProtection,
    _In_     ULONG              AllocationAttributes,
    _In_opt_ HANDLE             FileHandle);
extern "C" NTSTATUS NTAPI NtMapViewOfSection(
    _In_        HANDLE          SectionHandle,
    _In_        HANDLE          ProcessHandle,
    _Inout_     PVOID* BaseAddress,
    _In_        ULONG_PTR       ZeroBits,
    _In_        SIZE_T          CommitSize,
    _Inout_opt_ PLARGE_INTEGER  SectionOffset,
    _Inout_     PSIZE_T         ViewSize,
    _In_        SECTION_INHERIT InheritDisposition,
    _In_        ULONG           AllocationType,
    _In_        ULONG           Win32Protect);
extern "C" NTSTATUS NTAPI NtUnmapViewOfSection(_In_ HANDLE ProcessHandle, _In_opt_ PVOID BaseAddress);
extern "C" NTSTATUS NTAPI NtClose(_In_ HANDLE Handle);

static ULONG characteristics_protection_map[2][2][2] =
{
    {
        { PAGE_NOACCESS, PAGE_WRITECOPY },
        { PAGE_READONLY, PAGE_READWRITE }

    },
    {
        { PAGE_EXECUTE, PAGE_EXECUTE_WRITECOPY },
        { PAGE_EXECUTE_READ, PAGE_EXECUTE_READWRITE }
    },
};


bool RemapVerifyPeSectionAlignment(PIMAGE_NT_HEADERS p_nt_headers)
{
    SYSTEM_INFO systemInfo = {};
    PIMAGE_SECTION_HEADER p_section_header = NULL;
    ULONG_PTR section_base = 0;

    std::cout << "[BAC]:Verfying pe section alignment:" << std::endl;

    //查询系统信息
    GetSystemInfo(&systemInfo);

    p_section_header = IMAGE_FIRST_SECTION(p_nt_headers);

    for (WORD i = 0; i < p_nt_headers->FileHeader.NumberOfSections; ++i)
    {
        section_base =
            p_nt_headers->OptionalHeader.ImageBase +
            p_section_header[i].VirtualAddress;

        printf("[BAC]:%-8.8s    0x%IX - 0x%IX,  0x%08X\n", 
            p_section_header[i].Name,
            section_base,
            section_base + p_section_header[i].Misc.VirtualSize,
            p_section_header[i].Misc.VirtualSize);

        if (!POINTER_IS_ALIGNED(section_base, systemInfo.dwAllocationGranularity))
        {
            printf("[BAC]:Unexpected section alignment. (section_base = 0x%IX)\n", section_base);
            return false;
        }
    }

    //验证PE头
    if (!POINTER_IS_ALIGNED(p_nt_headers->OptionalHeader.ImageBase, systemInfo.dwAllocationGranularity))
    {
        printf("[BAC]:Unexpected section alignment. (section_base = 0x%IX)\n", section_base);
        return false;
    }
}

void RemapCopyPeSecionts(PIMAGE_NT_HEADERS p_nt_headers, ULONG_PTR destination_base)
{
    ULONG_PTR source_base = 0;
    PIMAGE_SECTION_HEADER p_section_header = NULL;

    source_base = p_nt_headers->OptionalHeader.ImageBase;

    p_section_header = IMAGE_FIRST_SECTION(p_nt_headers);

    //单独复制每个pe部分，因为使用ALIGN' link选项编译的图像将保留内存填充。
    for (WORD i = 0; i < p_nt_headers->FileHeader.NumberOfSections; ++i)
    {
        printf("[BAC]:Copying %-8.8s from 0x%IX to 0x%IX,  0x%08X\n",
            p_section_header[i].Name,
            source_base + p_section_header[i].VirtualAddress,
            destination_base + p_section_header[i].VirtualAddress,
            p_section_header[i].Misc.VirtualSize);

        RtlCopyMemory(
            (PVOID)(destination_base + p_section_header[i].VirtualAddress),
            (PVOID)(source_base + p_section_header[i].VirtualAddress),
            p_section_header[i].Misc.VirtualSize);
    }

    //拷贝PE头
    RtlCopyMemory((PVOID)destination_base, (PVOID)source_base, PAGE_SIZE);
}

ULONG RemapConvertSectionCharacteristicsToPageProtection(ULONG characteristics)
{
    BOOL f_executable = FALSE;
    BOOL f_readable = FALSE;
    BOOL f_writable = FALSE;
    ULONG protection = 0;

    if (0 != (IMAGE_SCN_MEM_EXECUTE & characteristics))
        f_executable = TRUE;

    if (0 != (IMAGE_SCN_MEM_READ & characteristics))
        f_readable = TRUE;

    if (0 != (IMAGE_SCN_MEM_WRITE & characteristics))
        f_writable = TRUE;

    protection = characteristics_protection_map[f_executable][f_readable][f_writable];

    if (0 != (IMAGE_SCN_MEM_NOT_CACHED & characteristics))
        protection |= PAGE_NOCACHE;

    return protection;
}

BOOL RemapProtectedView(HANDLE h_section, ULONG_PTR base_address, SIZE_T cbSize, SIZE_T cb_offset, ULONG protection)
{
    LARGE_INTEGER cb_section_offset = {};
    PVOID p_view_base = NULL;
    SIZE_T cb_view_size = 0;

    p_view_base = (PVOID)base_address;
    cb_view_size = cbSize;
    cb_section_offset.QuadPart = cb_offset;

    if (!NT_SUCCESS(NtMapViewOfSection(h_section, NtCurrentProcess(), &p_view_base, 0, 0,
        &cb_section_offset, &cb_view_size, ViewUnmap, SEC_NO_CHANGE, protection)))
    {
        printf("[BAC]:NtMapViewOfSection failed: 0x%X (Base = 0x%IX, Offset = 0x%IX, Size = 0x%IX)\n",
            0,
            p_view_base,
            cb_section_offset.QuadPart,
            cb_view_size);
        return false;
    }

    return true;
}

BOOL RemapImageRoutine(PVOID p_remap_region)
{
    PIMAGE_NT_HEADERS p_nt_headers = NULL;
    HANDLE h_section = NULL;
    LARGE_INTEGER cb_section_size = {};
    PVOID p_view_base = NULL;
    ULONG_PTR image_base = 0;
    LARGE_INTEGER cb_section_offset = {};
    SIZE_T cb_view_size = 0;
    PIMAGE_SECTION_HEADER p_section_header = NULL;
    ULONG protection = 0;

    p_nt_headers = RtlImageNtHeader(p_remap_region);
    if (!p_nt_headers)
    {
        printf("RtlImageNtHeader failed. (BaseAddress = 0x%IX)\n", p_remap_region);

        if (h_section)
        {
            if (!NT_SUCCESS(NtClose(h_section)))
                printf("NtClose failed\n");
        }
        return false;
    }

    cb_section_size.QuadPart = p_nt_headers->OptionalHeader.SizeOfImage;

    // Create a page-file-backed section to store the remapped image.
    if (!NT_SUCCESS(NtCreateSection(&h_section, SECTION_ALL_ACCESS, NULL, &cb_section_size,
        PAGE_EXECUTE_READWRITE, SEC_COMMIT | SEC_NO_CHANGE, NULL)))
    {
        printf("[BAC]:NtCreateSection failed\n");

        if (h_section)
        {
            if (!NT_SUCCESS(NtClose(h_section)))
                printf("NtClose failed\n");
        }
        return false;
    }

    // Map a view of the entire section.
    if (!NT_SUCCESS(NtMapViewOfSection(h_section, NtCurrentProcess(), &p_view_base, 0,
        p_nt_headers->OptionalHeader.SizeOfImage, &cb_section_offset, &cb_view_size, ViewUnmap, 0, PAGE_READWRITE)))
    {
        printf("[BAC]:NtMapViewOfSection failed\n");

        if (h_section)
        {
            if (!NT_SUCCESS(NtClose(h_section)))
                printf("NtClose failed\n");
        }
        return false;
    }

    // Copy the image to our view.
    RemapCopyPeSecionts(p_nt_headers, (ULONG_PTR)p_view_base);

    // Unmap the copy-view because we no longer need it.
    if (!NT_SUCCESS(NtUnmapViewOfSection(NtCurrentProcess(), p_view_base)))
    {
        printf("NtUnmapViewOfSection failed\n");
        if (h_section)
        {
            if (!NT_SUCCESS(NtClose(h_section)))
                printf("NtClose failed\n");
        }
        return false;
    }

    // Unmap the original image.
    image_base = p_nt_headers->OptionalHeader.ImageBase;

    if (!NT_SUCCESS(NtUnmapViewOfSection(NtCurrentProcess(), (PVOID)image_base)))
    {
        printf("NtUnmapViewOfSection failed\n");
        if (h_section)
        {
            if (!NT_SUCCESS(NtClose(h_section)))
                printf("NtClose failed\n");
        }
        return false;
    }

    // Reconstruct the image by mapping a view of the section for each pe section in the image.
    p_section_header = IMAGE_FIRST_SECTION(p_nt_headers);

    for (WORD i = 0; i < p_nt_headers->FileHeader.NumberOfSections; ++i)
    {
        protection = RemapConvertSectionCharacteristicsToPageProtection(
            p_section_header[i].Characteristics);

        if (!RemapProtectedView(h_section, image_base + p_section_header[i].VirtualAddress,
            p_section_header[i].Misc.VirtualSize, p_section_header[i].VirtualAddress, protection))
        {
            printf("RmppMapProtectedView failed.\n");
            if (h_section)
            {
                if (!NT_SUCCESS(NtClose(h_section)))
                    printf("NtClose failed\n");
            }
            return false;
        }
    }

    // Map a view for the pe header.
    if (!RemapProtectedView(h_section, image_base, PAGE_SIZE, 0, PAGE_READONLY))
    {
        printf("RmppMapProtectedView failed.\n");
        goto exit;
    }

exit:
    if (h_section)
    {
        if (!NT_SUCCESS(NtClose(h_section)))
            printf("NtClose failed\n");
    }
    return true;
}

BOOL RemapValidateRemappedPeSectionProtection(PVOID p_section_base)
{
    MEMORY_BASIC_INFORMATION memory_basic_info = {};
    ULONG test_protect = 0;
    ULONG previous_protect = 0;
    BOOL status = TRUE;

    // Query the current page protection.
    if (!VirtualQuery(p_section_base, &memory_basic_info, sizeof(memory_basic_info)))
    {
        printf("VirtualQuery failed: %u (BaseAddress = 0x%IX)\n",
            GetLastError(),
            p_section_base);
        status = FALSE;
        goto exit;
    }

    // We use PAGE_EXECUTE_READWRITE as our test protection value because it is the most permissive page protection.
    if (PAGE_EXECUTE_READWRITE != memory_basic_info.Protect)
        test_protect = PAGE_EXECUTE_READWRITE;
    else
        test_protect = PAGE_NOACCESS;

    // Attempt to modify the page protection of every page contained in the
    //  memory region. This should fail because each view was mapped with the
    //  SEC_NO_CHANGE allocation type.
    if (VirtualProtect(p_section_base, memory_basic_info.RegionSize, test_protect, &previous_protect))
    {
        printf("Section is not protected. (BaseAddress = 0x%IX, Protect = 0x%X)\n",
            p_section_base,
            previous_protect);
        status = FALSE;
        goto exit;
    }

    // Reset the status code to indicate success.
    status = TRUE;

exit:
    return status;
}

BOOL RemapValidateRemappedImageProtection(ULONG_PTR image_base)
{
    PIMAGE_NT_HEADERS p_nt_headers = NULL;
    PIMAGE_SECTION_HEADER p_section_header = NULL;

    printf("[BAC]:Validating remapped image protection.\n");

    p_nt_headers = RtlImageNtHeader((PVOID)image_base);
    if (!p_nt_headers)
    {
        printf("[BAC]:RtlImageNtHeader failed. (BaseAddress = 0x%IX)\n", image_base);
        return false;
    }

    p_section_header = IMAGE_FIRST_SECTION(p_nt_headers);

    for (WORD i = 0; i < p_nt_headers->FileHeader.NumberOfSections; ++i)
    {
        if (!RemapValidateRemappedPeSectionProtection((PVOID)(image_base + p_section_header[i].VirtualAddress)))
        {
            printf("[BAC]:RmppValidateRemappedPeSectionProtection failed.\n");
            return false;
        }
    }

    // Validate the pe header.
    if (!RemapValidateRemappedPeSectionProtection((PVOID)image_base))
    {
        printf("[BAC]:RmppValidateRemappedPeSectionProtection failed.\n");
        return false;
    }

    return true;
}

bool BAC::RemapImage(ULONG_PTR image_base)
{
#if NDEBUG
	VMProtectBeginUltra("BAC::RemapImage");
#endif

	PIMAGE_NT_HEADERS p_nt_headers = nullptr;
	PVOID p_remap_region = nullptr;
	REMAP_ROUTINE fp_remap_routine = nullptr;

	printf("\nremapping image base:%p\n", image_base);

	p_nt_headers = RtlImageNtHeader((PVOID)image_base);
	if (!p_nt_headers)
	{
		baclog->FileLogf("RtlImageNtHeader failed, image base:%p", image_base);
		return false;
	}

    if (!RemapVerifyPeSectionAlignment(p_nt_headers))
        return false;

    //分配一个可执行和可写的缓冲区，在其中重新映射执行代码
    p_remap_region = ::VirtualAlloc(NULL, p_nt_headers->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!p_remap_region)
    {
        baclog->FileLogf("VritualAlloc failed:%u", ::GetLastError());
        return false;
    }
    printf("[BAC]:remap region:%p\n", p_remap_region);

    //拷贝image
    RemapCopyPeSecionts(p_nt_headers, (ULONG_PTR)p_remap_region);

    //在重映射区域内定位重映射例程的地址
    fp_remap_routine = (REMAP_ROUTINE)((ULONG_PTR)p_remap_region + (ULONG_PTR)RemapImageRoutine - image_base);
    printf("[BAC]: remap routine:%p\n", fp_remap_routine);
    if (!fp_remap_routine(p_remap_region))
    {
        baclog->FileLog("Remap image routine failed");
        if (p_remap_region)
            ::VirtualFree(p_remap_region, 0, MEM_RELEASE);
        return false;
    }

    if (!RemapValidateRemappedImageProtection(image_base))
    {
        baclog->FileLog("RemapValidateRemappedImageProtection failed");
        if (p_remap_region)
            ::VirtualFree(p_remap_region, 0, MEM_RELEASE);
        return false;
    }

    if (p_remap_region)
        ::VirtualFree(p_remap_region, 0, MEM_RELEASE);
	return true;
#if NDEBUG
	VMProtectEnd();
#endif
}
