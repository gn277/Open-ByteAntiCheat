#pragma once
#include <ntifs.h>

typedef struct _OB_REG_CONTEXT
{
	USHORT Version;
	UNICODE_STRING Altitude;
	USHORT ulIndex;
	OB_OPERATION_REGISTRATION* OperationRegistration;
} REG_CONTEXT, * PREG_CONTEXT;


class ProcessProtect
{
private:
	PDRIVER_OBJECT _p_driver_object = nullptr;
	char _protect_process_name[100] = { NULL };
	HANDLE _callbacks_handle = NULL;

private:
	__int64 ExpLookupHandleTableEntry(unsigned int* a1, __int64 a2);
	NTSTATUS RegisterProtectProcessCallbacks();

public:
	void* operator new(size_t size, POOL_TYPE pool_type = NonPagedPool);
	void operator delete(void* pointer);

public:
	ProcessProtect(PDRIVER_OBJECT p_driver_object);
	~ProcessProtect();

public:
	char* GetProtectProcessName();
	HANDLE GetProcessID(const char* process_name);
	bool JudgmentProtectProcess(PEPROCESS p_current_eproc);
	static OB_PREOP_CALLBACK_STATUS ProcessHandlePreCallback(PVOID registration_context, POB_PRE_OPERATION_INFORMATION operation_information);
	static OB_PREOP_CALLBACK_STATUS ThreadHandlePreCallback(PVOID registration_context, POB_PRE_OPERATION_INFORMATION operation_information);

public:
	NTSTATUS ProtectProcess(const char* process_name);

};

