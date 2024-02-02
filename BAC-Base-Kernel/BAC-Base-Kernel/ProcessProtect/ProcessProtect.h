#pragma once
#include <ntifs.h>


class ProcessProtect
{
private:
	char _protect_process_name[100] = { NULL };

private:
	__int64 ExpLookupHandleTableEntry(unsigned int* a1, __int64 a2);

public:
	void* operator new(size_t size, POOL_TYPE pool_type = NonPagedPool);
	void operator delete(void* pointer);

public:
	ProcessProtect();
	~ProcessProtect();

public:
	char* GetProtectProcessName();
	static OB_PREOP_CALLBACK_STATUS ProcHandleAccessCallback(PVOID registration_context, POB_PRE_OPERATION_INFORMATION p_ob_pre_operation_info);

public:
	NTSTATUS ProtectProcess(const char* process_name);

};

