#pragma once
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include <Psapi.h>
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")

typedef struct _CallerStackInfo
{
	std::string caller_module_name;
	HMODULE caller_module_handle;
	PVOID caller_address;
	SIZE_T caller_offset;
}CallerStackInfo, * PCallerStackInfo;


class Tools
{
private:

public:
	Tools();
	~Tools();

public:
	PVOID64 GetModuleEndAddress(HMODULE module_handle);

public:
	ULONG64 ReadULong64(PVOID address);
	LONG64 ReadLong64(PVOID address);
	ULONG ReadULong(PVOID address);
	UINT ReadUInt(PVOID address);
	INT ReadInt(PVOID address);
	BYTE ReadByte(PVOID address);

public:
	PVOID GetPeSectiontAddress(PVOID image, const char* section_name);
	DWORD GetPeSectionSize(PVOID image, const char* section_name);
	bool GetProcessModule(std::map<std::string, DWORD64>* p_process_module_list);

private:
	CallerStackInfo GetMemoryModuleInfo(PVOID address);

public:
	void GetStackInfoList();

};


