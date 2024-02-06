////////////////////////////////////////////////////////////////
//				这里定义内核与应用层共用的数据结构
////////////////////////////////////////////////////////////////
#pragma once

#define DRIVER_NAME L"\\Device\\BACDriver"
#define DRIVER_LINKER_NAME L"\\??\\BACLinker"


typedef struct _ProtectProcessStruct
{
	wchar_t file_path[MAX_PATH];
}ProtectProcessStruct, * PProtectProcessStruct;

typedef struct _ProtectMemoryStruct
{
	char module_name[MAX_PATH];
	HANDLE process_id;
	DWORD64 memory_address;
	DWORD memory_size;
}ProtectMemoryStruct, * PProtectMemoryStruct;



