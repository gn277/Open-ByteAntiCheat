#include "BACTools.h"
//#include "../BACInterface/BAC.h"
#include "../BAC-Base.h"


Tools::Tools()
{
}

Tools::~Tools()
{
}

PVOID64 Tools::GetModuleEndAddress(HMODULE module_handle)
{
	PVOID64 return_address = 0;
	MODULEINFO moduleinfo = { 0 };

	GetModuleInformation(::GetCurrentProcess(), module_handle, &moduleinfo, sizeof(MODULEINFO));

	return return_address = (PVOID64)((__int64)moduleinfo.lpBaseOfDll + moduleinfo.SizeOfImage);
}

ULONG64 Tools::ReadULong64(PVOID address)
{
	if (IsBadReadPtr(address, 8))
		return 0;

	return *(ULONG64*)address;
}

LONG64 Tools::ReadLong64(PVOID address)
{
	if (IsBadReadPtr(address, 8))
		return 0;

	return *(LONG64*)address;
}

ULONG Tools::ReadULong(PVOID address)
{
	if (IsBadReadPtr(address, 4))
		return 0;

	return *(ULONG*)address;
}

UINT Tools::ReadUInt(PVOID address)
{
	if (IsBadReadPtr(address, 4))
		return 0;

	return *(UINT*)address;
}

INT Tools::ReadInt(PVOID address)
{
	if (IsBadReadPtr(address, 4))
		return 0;

	return *(INT*)address;
}

BYTE Tools::ReadByte(PVOID address)
{
	if (IsBadReadPtr(address, sizeof(BYTE)))
		return 0;

	return *(BYTE*)address;
}

PVOID Tools::GetPeSectiontAddress(PVOID image, const char* section_name)
{
	PIMAGE_NT_HEADERS p_nt_header = (PIMAGE_NT_HEADERS)((ULONG_PTR)image + ((PIMAGE_DOS_HEADER)image)->e_lfanew);
	if (!p_nt_header)
		return 0;

	PIMAGE_OPTIONAL_HEADER p_option_header = (PIMAGE_OPTIONAL_HEADER)(&(p_nt_header->OptionalHeader));
	PIMAGE_FILE_HEADER p_file_header = (PIMAGE_FILE_HEADER)(&(p_nt_header->FileHeader));
	PIMAGE_SECTION_HEADER p_section_header = (PIMAGE_SECTION_HEADER)((char*)p_option_header + p_file_header->SizeOfOptionalHeader);

	//遍历节
	for (WORD i = 0; i < p_nt_header->FileHeader.NumberOfSections; ++i)
	{
		DWORD64 section_base = (p_nt_header->OptionalHeader.ImageBase + p_section_header[i].VirtualAddress);

		if (_stricmp(section_name, (const char*)p_section_header[i].Name) == 0)
		{
			//printf("    %-8.8s    0x%IX - 0x%IX,  0x%08X\n",
			//	p_section_header[i].Name,
			//	section_base,
			//	section_base + p_section_header[i].Misc.VirtualSize,
			//	p_section_header[i].Misc.VirtualSize);

			return (PVOID)section_base;
		}
	}

	return 0;
}

DWORD Tools::GetPeSectionSize(PVOID image, const char* section_name)
{
	PIMAGE_NT_HEADERS p_nt_header = (PIMAGE_NT_HEADERS)((ULONG_PTR)image + ((PIMAGE_DOS_HEADER)image)->e_lfanew);
	if (!p_nt_header)
		return 0;

	PIMAGE_OPTIONAL_HEADER p_option_header = (PIMAGE_OPTIONAL_HEADER)(&(p_nt_header->OptionalHeader));
	PIMAGE_FILE_HEADER p_file_header = (PIMAGE_FILE_HEADER)(&(p_nt_header->FileHeader));
	PIMAGE_SECTION_HEADER p_section_header = (PIMAGE_SECTION_HEADER)((char*)p_option_header + p_file_header->SizeOfOptionalHeader);

	//遍历节
	for (WORD i = 0; i < p_nt_header->FileHeader.NumberOfSections; ++i)
	{
		DWORD64 section_base = (p_nt_header->OptionalHeader.ImageBase + p_section_header[i].VirtualAddress);

		if (_stricmp(section_name, (const char*)p_section_header[i].Name) == 0)
			return p_section_header[i].Misc.VirtualSize;
	}

	return 0;
}

bool Tools::GetProcessModule(std::map<std::string, DWORD64>* p_process_module_list)
{
#if NDEBUG
	VMProtectBeginUltra("Tools::GetProcessModule");
#endif
	baclog->FunctionLog(__FUNCTION__, "Enter");

	HMODULE* p_module = NULL;
	HANDLE handle = NULL;
	DWORD dw_needed = 0;
	DWORD i = 0;
	CHAR module_name[MAX_PATH] = { NULL };

	handle = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ::GetCurrentProcessId());
	if (NULL == handle)
	{
		baclog->FileLogf("can't open process[ID:0x%x]handle,error code:0x%08x\n", ::GetCurrentProcessId(), ::GetLastError());
		return false;
	}

	EnumProcessModules(handle, NULL, 0, &dw_needed);
	p_module = (HMODULE*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ::GetCurrentProcessId());

	if (EnumProcessModules(handle, p_module, dw_needed, &dw_needed))
	{
		for (i = 0; i < (dw_needed / sizeof(HMODULE)); i++)
		{
			ZeroMemory(module_name, MAX_PATH);
			//在这如果使用GetModuleFileName，有的模块名称获取不到，函数返回无法找到该模块的错误
			if (GetModuleFileNameExA(handle, p_module[i], module_name, MAX_PATH))
			{
				//printf("模块名：%s,模块句柄：%p\n", module_name, p_module[i]);
				p_process_module_list->insert(std::make_pair(module_name, (DWORD64)p_module[i]));
			}
		}
	}

	baclog->FunctionLog(__FUNCTION__, "Leave");
	HeapFree(GetProcessHeap(), 0, p_module);
	CloseHandle(handle);
	return true;
#if NDEBUG
	VMProtectEnd();
#endif
}

bool Tools::StackTrace64()
{
	CONTEXT                       context;
	KNONVOLATILE_CONTEXT_POINTERS nt_context;
	UNWIND_HISTORY_TABLE          unwind_history_table;
	PRUNTIME_FUNCTION             runtime_function;
	PVOID                         handler_data;
	ULONG64                       establisher_frame;
	ULONG64                       image_base;

	//首先得到当前context
	RtlCaptureContext(&context);

	//初始化 unwind_history_table，这个结构体主要用于多次查找RUNTIME_FUNCTION时加快查找效率
	RtlZeroMemory(&unwind_history_table, sizeof(UNWIND_HISTORY_TABLE));

	//循环得到调用堆栈
	for (ULONG Frame = 0; ; Frame++)
	{
		//在PE+ 的.pdata 段中找到函数对应的runtime_function 结构
		//只有叶函数没有此结构
		//既不调用函数、又没有修改栈指针，也没有使用 SEH 的函数就叫做“叶函数”。
		runtime_function = RtlLookupFunctionEntry(context.Rip, &image_base, &unwind_history_table);

		RtlZeroMemory(&nt_context, sizeof(KNONVOLATILE_CONTEXT_POINTERS));

		if (!runtime_function)
		{
			//没有得到结构体，我们当前得到了一个叶函数
			//此时Rsp 直接指向Rip，调用叶函数只包含Call 操作，即只包含push eip 操作，Rsp += 8即可
			context.Rip = (ULONG64)(*(PULONG64)context.Rsp);
			context.Rsp += 8;
		}
		else
		{
			//虚拟展开，得到调用堆栈
			//第一个参数表示Rip 所在函数没有过滤和处理函数
			//仅仅进行虚拟展开得到上层调用堆栈
			RtlVirtualUnwind(
				UNW_FLAG_NHANDLER,
				image_base,
				context.Rip,
				runtime_function,
				&context,
				&handler_data,
				&establisher_frame,
				&nt_context);
		}

		//没有得到Rip 即是调用失败
		if (!context.Rip)
			break;

		//展示相关信息
		printf("FRAME %02x: CallAddress=%p\r\n\n", Frame, context.Rip);
	}

	return true;
}

