#include "BACTools.h"


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
	//baclog->FunctionLog(__FUNCTION__, "Enter");

	HMODULE* p_module = NULL;
	HANDLE handle = NULL;
	DWORD dw_needed = 0;
	DWORD i = 0;
	CHAR module_name[MAX_PATH] = { NULL };

	handle = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ::GetCurrentProcessId());
	if (NULL == handle)
	{
		//baclog->FileLogf("can't open process[ID:0x%x]handle,error code:0x%08x\n", ::GetCurrentProcessId(), ::GetLastError());
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

	//baclog->FunctionLog(__FUNCTION__, "Leave");
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

std::string BACTools::ToHexString(std::string s)
{
	std::ostringstream out;

	out << std::hex << std::setw(2) << std::setfill('0');
	for (size_t i = 0; i < s.size(); i++)
		out << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>((unsigned char)s.data()[i]);

	return out.str();
}

std::string BACTools::ToHexString(unsigned char* s, size_t len)
{
	std::ostringstream out;

	out << std::hex << std::setw(2) << std::setfill('0');
	for (size_t i = 0; i < len; i++)
		out << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(s[i]);

	return out.str();
}

std::string BACTools::NumberToHexString(void* i, unsigned int len)
{
	std::stringstream stream;

	stream << std::setfill('0') << std::setw(len * 2)
		<< std::hex << i;

	return stream.str();
}

std::string BACTools::ToBytes(void* data, unsigned int len)
{
	std::string stringb;
	char* bytes = new char[len];

	memcpy(bytes, &data, len);
	stringb.append(bytes, len);

	bytes = nullptr;
	delete[] bytes;

	return stringb;
}

std::wstring BACTools::StringToWString(const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(str);
}

std::string BACTools::WStringToString(std::wstring& wide_string)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
	return utf8_conv.to_bytes(wide_string);
}

int BACTools::Hex2Dec(char ch)
{
	if (ch >= '0' && ch <= '9')
		return ch - '0';
	else if (ch >= 'A' && ch <= 'Z')
		return ch - 'A' + 10;
	else if (ch >= 'a' && ch <= 'z')
		return ch - 'z' + 10;
	else
		return -1;
}

unsigned long long BACTools::HexToDecTwo2(const char* hex_str)
{
	unsigned long long num = 0;
	unsigned long long temp;
	int bits;
	int i;

	int len = (int)strlen(hex_str);

	for (i = 0, temp = 0; i < len; i++, temp = 0)
	{
		temp = this->Hex2Dec(*(hex_str + i));
		bits = (len - i - 1) * 4;
		temp = temp << bits;
		num = num | temp;
	}
	return num;
}

unsigned char* BACTools::EndianSwap(unsigned char* pData, int startIndex, int length)
{
	int i, cnt, end, start;
	cnt = length / 2;
	start = startIndex;
	end = startIndex + length - 1;
	unsigned char tmp;

	for (i = 0; i < cnt; i++)
	{
		tmp = pData[start + i];
		pData[start + i] = pData[end - i];
		pData[end - i] = tmp;
	}

	return pData;
}

unsigned int* BACTools::SmallToBig(unsigned int* value)
{
	char tmp = '\0';
	char* ptr = (char*)value;

	tmp = *ptr;
	*ptr = *(ptr + 3);
	*(ptr + 3) = tmp;
	tmp = *(ptr + 1);
	*(ptr + 1) = *(ptr + 2);
	*(ptr + 2) = tmp;

	return value;
}

bool BACTools::ReadFileToMemory(std::string file_path, std::string* p_buffer, unsigned long long read_offset, unsigned long long read_len, int mode)
{
	std::ifstream file(file_path, mode);
	unsigned long long read_file_len = read_len;

	if (!file.is_open())
		return false;

	if (read_offset == 0)
	{
		//获取文件大小
		file.seekg(0, std::ifstream::end);
		read_file_len = file.tellg();
		file.seekg(0);
	}
	else
	{
		//根据想要读取的偏移获取对应长度数据
		file.seekg(read_offset, std::ios::beg);
	}

	//读取文件数据
	char* temp_buffer = new char[read_file_len];

	file.read(temp_buffer, read_file_len);
	p_buffer->append(temp_buffer, read_file_len);

	file.close();
	delete[] temp_buffer;

	return true;
}

bool BACTools::WriteMemoryToFile(std::string file_path, std::string buffer, int mode)
{
	std::ofstream file(file_path, mode);

	if (!file.is_open())
		return false;

	//写入文件数据
	file.write(buffer.data(), buffer.size());
	file.close();

	return true;
}

bool BACTools::CreateFile_(std::string file_path, int mode)
{
	std::fstream file(file_path, mode);

	if (!file.is_open())
		return false;

	file.close();

	return true;
}

