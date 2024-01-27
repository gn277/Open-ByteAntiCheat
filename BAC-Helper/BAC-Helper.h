#pragma once
#include <Windows.h>
#include <iostream>

#include "../BAC-Base/ProtectSDK/VMProtectSDK/VMProtectSDK.h"
#if _WIN64
#pragma comment(lib,"../BAC-Base/ProtectSDK/VMProtectSDK/VMProtectSDK64.lib")
#elif _WIN32
#pragma comment(lib,"../BAC-Base/ProtectSDK/VMProtectSDK/VMProtectSDK32.lib")
#else
#error "Unsupported platforms"
#endif


class BACHelper
{
private:

public:
	BACHelper();
	~BACHelper();

	//PE注入：向游戏主进程中注入BAC-Base.dll以达到主动加载反作弊的目的
public:
	bool ImportTableInject(const char* target_exe_path, const char* new_exe_path, const char* dll_name, const char* import_function_name);

private:
	LPVOID ReadFileToBuffer(const char* file_path);
	DWORD* AddSection(char* file_buffer, DWORD section_size, DWORD virtual_size);
	DWORD RVAtoFOA(DWORD roa_address, char* file_buffer);
#if _WIN64
	DWORD64 FOAtoRVA(char* file_buffer, DWORD64 foa_address);
#else
	DWORD FOAtoRVA(char* file_buffer, DWORD foa_address);
#endif

};

