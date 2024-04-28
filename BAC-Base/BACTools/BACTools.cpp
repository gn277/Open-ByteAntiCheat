#include "BACTools.h"
//#include "../BACInterface/BAC.h"
#include "../BAC-Base.h"


BACTools::BACTools()
{
}

BACTools::~BACTools()
{
}

PVOID64 BACTools::GetModuleEndAddress(HMODULE module_handle)
{
	PVOID64 return_address = 0;
	MODULEINFO moduleinfo = { 0 };

	GetModuleInformation(::GetCurrentProcess(), module_handle, &moduleinfo, sizeof(MODULEINFO));

	return return_address = (PVOID64)((__int64)moduleinfo.lpBaseOfDll + moduleinfo.SizeOfImage);
}

ULONG64 BACTools::ReadULong64(PVOID address)
{
	if (IsBadReadPtr(address, 8))
		return 0;

	return *(ULONG64*)address;
}

LONG64 BACTools::ReadLong64(PVOID address)
{
	if (IsBadReadPtr(address, 8))
		return 0;

	return *(LONG64*)address;
}

ULONG BACTools::ReadULong(PVOID address)
{
	if (IsBadReadPtr(address, 4))
		return 0;

	return *(ULONG*)address;
}

UINT BACTools::ReadUInt(PVOID address)
{
	if (IsBadReadPtr(address, 4))
		return 0;

	return *(UINT*)address;
}

INT BACTools::ReadInt(PVOID address)
{
	if (IsBadReadPtr(address, 4))
		return 0;

	return *(INT*)address;
}

BYTE BACTools::ReadByte(PVOID address)
{
	if (IsBadReadPtr(address, sizeof(BYTE)))
		return 0;

	return *(BYTE*)address;
}

PVOID BACTools::GetPeSectiontAddress(PVOID image, const char* section_name)
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

DWORD BACTools::GetPeSectionSize(PVOID image, const char* section_name)
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

bool BACTools::GetProcessModule(std::map<std::string, DWORD64>* p_process_module_list)
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

CallerStackInfo BACTools::GetMemoryModuleInfo(PVOID address)
{
	MODULEINFO module_info = { NULL };
	MEMORY_BASIC_INFORMATION memory_info = { NULL };
	CallerStackInfo caller_info;

	//记录调用者地址
	caller_info.caller_address = address;

	//获取调用者所在模块
	VirtualQuery(address, &memory_info, sizeof(MEMORY_BASIC_INFORMATION));
	caller_info.caller_module_handle = (HMODULE)memory_info.AllocationBase;

	//计算调用者所在模块偏移
#if _WIN64
	caller_info.caller_offset = ((DWORD64)caller_info.caller_address - (DWORD64)caller_info.caller_module_handle);
#else
	caller_info.caller_offset = ((DWORD)caller_info.caller_address - (DWORD)caller_info.caller_module_handle);
#endif

	//保存调用者模块名称
	char module_name[MAX_PATH] = { NULL };
	GetModuleBaseNameA(::GetCurrentProcess(), (HMODULE)memory_info.AllocationBase, module_name, MAX_PATH);
	caller_info.caller_module_name.append(module_name);

	return caller_info;
}

void BACTools::GetStackInfoList()
{
	//// Quote from Microsoft Documentation:
	//// ## Windows Server 2003 and Windows XP:  
	//// ## The sum of the FramesToSkip and FramesToCapture parameters must be less than 63.
	//const int max_callers = 62;
	//void* callers_stack[max_callers];
	//
	//unsigned short frames;
	//SYMBOL_INFO* symbol;
	//HANDLE process = GetCurrentProcess();
	//SymInitialize(process, NULL, TRUE);
	//
	//frames = RtlCaptureStackBackTrace(0, max_callers, callers_stack, NULL);
	//symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
	//symbol->MaxNameLen = 255;
	//symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	//
	//const unsigned short  MAX_CALLERS_SHOWN = 6;
	//frames = frames < MAX_CALLERS_SHOWN ? frames : MAX_CALLERS_SHOWN;
	//for (unsigned int i = 0; i < frames; i++)
	//{
	//	SymFromAddr(process, (DWORD64)(callers_stack[i]), 0, symbol);
	//	//printf("*** %d: callers_stack:%p function_name:%s - function_address:%p\n",
	//	//	i, callers_stack[i], symbol->Name, symbol->Address);
	//	CallerStackInfo caller_info = GetMemoryModuleInfo(callers_stack[i]);
	//	printf("***%d: caller_module_name:%s caller_module_handle:%p caller_offset:%p caller_add:%p \n",
	//		i, caller_info.caller_module_name.c_str(), caller_info.caller_module_handle,
	//		caller_info.caller_offset, caller_info.caller_address);
	//}
	//
	//free(symbol);

	// Quote from Microsoft Documentation:
	// ## Windows Server 2003 and Windows XP:  
	// ## The sum of the FramesToSkip and FramesToCapture parameters must be less than 63.
	const int max_callers = 62;
	void* callers_stack[max_callers];

	unsigned short frames;

	frames = RtlCaptureStackBackTrace(0, max_callers, callers_stack, NULL);

	const unsigned short  MAX_CALLERS_SHOWN = 6;
	frames = frames < MAX_CALLERS_SHOWN ? frames : MAX_CALLERS_SHOWN;
	for (unsigned int i = 0; i < frames; i++)
	{
		CallerStackInfo caller_info = GetMemoryModuleInfo(callers_stack[i]);
		std::cout << "*** " << std::internal << i << " : caller: " << caller_info.caller_module_name << "+" << std::hex << caller_info.caller_offset << std::endl;
	}
}

