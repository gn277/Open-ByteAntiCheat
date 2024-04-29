#pragma once
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <codecvt> // codecvt_utf8
#include <locale>  // wstring_convert
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


class BACTools
{
private:

public:
	BACTools();
	~BACTools();

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

public:
	std::string ToHexString(std::string s);
	std::string ToHexString(unsigned char* s, size_t len);
	std::string NumberToHexString(void* i, unsigned int len);
	std::string ToBytes(void* data, unsigned int len);
	std::wstring StringToWString(const std::string& str);
	std::string WStringToString(std::wstring& wide_string);

	int Hex2Dec(char ch);
	unsigned long long HexToDecTwo2(const char* hex_str);

public:
	//将下标为startIndex开始，长度为length的这段数据进行大小端转换
	virtual unsigned char* EndianSwap(unsigned char* pData, int startIndex, int length);
	virtual unsigned int* SmallToBig(unsigned int* value);

public:
	virtual bool ReadFileToMemory(std::string file_path, std::string* p_buffer, unsigned long long read_offset = 0, unsigned long long read_len = 0, int mode = std::ios::in | std::ios::binary);
	virtual bool WriteMemoryToFile(std::string file_path, std::string buffer, int mode = std::ios::out | std::ios::binary);
	virtual bool CreateFile_(std::string file_path, int mode = std::ios::in | std::ios::out);

};


