#pragma once
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <winternl.h>
#include <Psapi.h>
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")


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

};


