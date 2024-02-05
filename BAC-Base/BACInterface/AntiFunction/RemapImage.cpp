////////////////////////////////////////////////////////////////
//						ƒ⁄¥Ê÷ÿ”≥…‰
////////////////////////////////////////////////////////////////
#include "../BAC.h"
#include "../../BAC-Base.h"


typedef enum _SECTION_INHERIT
{
    ViewShare = 1,
    ViewUnmap = 2
} SECTION_INHERIT;
typedef  DWORD(WINAPI* pfZwCreateSectionProc)(
    PHANDLE SectionHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    PLARGE_INTEGER  MaximumSize,
    ULONG SectionPageProtection,
    ULONG AllocationAttributes,
    HANDLE FileHandle);
typedef DWORD(WINAPI* pfZwMapViewOfSectionProc)(
    HANDLE SectionHandle,
    HANDLE ProcessHandle,
    PVOID* BaseAddress,
    ULONG_PTR ZeroBits,
    SIZE_T CommitSize,
    PLARGE_INTEGER SectionOffset,
    PSIZE_T ViewSize,
    SECTION_INHERIT InheritDisposition,
    ULONG AllocationType,
    ULONG Win32Protect);
typedef NTSTATUS(NTAPI* pfZwUnmapViewOfSection)(
    HANDLE ProcessHandle,
    PVOID BaseAddress);


bool BAC::RemapImage(ULONG_PTR image_base)
{
    HMODULE h_module = ::LoadLibraryA("ntdll.dll");

    pfZwCreateSectionProc pfnZwCreateSection = (pfZwCreateSectionProc)GetProcAddress(h_module, "NtCreateSection");
    pfZwMapViewOfSectionProc pfnZwMapViewOfSection = (pfZwMapViewOfSectionProc)::GetProcAddress(h_module, "ZwMapViewOfSection");
    pfZwUnmapViewOfSection pfnZwUnmpViewOfSection = (pfZwUnmapViewOfSection)::GetProcAddress(h_module, "ZwUnmapViewOfSection");

    MODULEINFO module_info = { NULL };
    ::GetModuleInformation(::GetCurrentProcess(), ::GetModuleHandleA("BAC-Base64.dll"), &module_info, sizeof(module_info));


    HANDLE section_handle;
    LARGE_INTEGER maximum_size = { NULL };
    PVOID base_address = module_info.lpBaseOfDll;
    maximum_size.QuadPart = module_info.SizeOfImage;
    OBJECT_ATTRIBUTES obj = { NULL };
    
    InitializeObjectAttributes(&obj, NULL, 0x40, 0, 0);
    DWORD error = pfnZwCreateSection(&section_handle, SECTION_ALL_ACCESS, NULL, &maximum_size, PAGE_EXECUTE_READ, SEC_COMMIT, NULL);
    
    pfnZwUnmpViewOfSection(::GetCurrentProcess(), base_address);

    SIZE_T view_size = 0;
    PVOID view_base = base_address;
    LARGE_INTEGER section_offset = { NULL };
    //÷ÿ”≥…‰
    error = pfnZwMapViewOfSection(section_handle, GetCurrentProcess(), &view_base, 0,
        module_info.SizeOfImage, &section_offset, &view_size, (SECTION_INHERIT)2, 0, PAGE_EXECUTE_READ);
    printf("base address:%p,base size:%d, section_handle:%p\n", base_address, module_info.SizeOfImage, section_handle);

    //memcpy(view_base, base_address, view_size);


    return true;
}



