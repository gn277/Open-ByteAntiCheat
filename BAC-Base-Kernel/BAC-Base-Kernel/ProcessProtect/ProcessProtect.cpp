#include "ProcessProtect.h"
#include "../../DriverEntry.h"


ProcessProtect::ProcessProtect()
{
}

ProcessProtect::~ProcessProtect()
{
}

void* ProcessProtect::operator new(size_t size, POOL_TYPE pool_type)
{
    return ExAllocatePool2(pool_type, size, 'bac1');
}

void ProcessProtect::operator delete(void* pointer)
{
    ExFreePoolWithTag(pointer, 'bac1');
}

__int64 ProcessProtect::ExpLookupHandleTableEntry(unsigned int* a1, __int64 a2)
{
	unsigned __int64 v2; // rdx
	__int64 v3; // r8

	v2 = a2 & 0xFFFFFFFFFFFFFFFC;
	if (v2 >= *a1)
		return 0;
	v3 = *((ULONG64*)a1 + 1);
	if ((v3 & 3) == 1)
		return *(ULONG64*)(v3 + 8 * (v2 >> 10) - 1) + 4 * (v2 & 0x3FF);
	if ((v3 & 3) != 0)
		return *(ULONG64*)(*(ULONG64*)(v3 + 8 * (v2 >> 19) - 2) + 8 * ((v2 >> 10) & 0x1FF)) + 4 * (v2 & 0x3FF);
	return v3 + 4 * v2;
}

char* ProcessProtect::GetProtectProcessName()
{
	return this->_protect_process_name;
}

OB_PREOP_CALLBACK_STATUS ProcessProtect::ProcHandleAccessCallback(PVOID registration_context, POB_PRE_OPERATION_INFORMATION p_ob_pre_operation_info)
{
	(registration_context);
	if (p_ob_pre_operation_info->Operation == OB_OPERATION_HANDLE_CREATE && *PsProcessType == p_ob_pre_operation_info->ObjectType)
	{
		PEPROCESS pEProcess = (PEPROCESS)p_ob_pre_operation_info->Object;
		// 判断操作类型是创建句柄
		//if (isProtectProcess(pEProcess)) // 是否是要保护的进程
		//{
		//	p_ob_pre_operation_info->Parameters->CreateHandleInformation.DesiredAccess = 0x1fffff;
		//}
		bac->ProtectProcess("Test");
	}
	return OB_PREOP_SUCCESS;
}

NTSTATUS ProcessProtect::ProtectProcess(const char* process_name)
{


	return STATUS_SUCCESS;
}



