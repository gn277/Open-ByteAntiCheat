#include "ProcessProtect.h"
#include "../../DriverEntry.h"

//char protect_process_name[100] = { NULL };

//申明函数
extern "C" PCHAR PsGetProcessImageFileName(IN PEPROCESS p_process);


ProcessProtect::ProcessProtect(PDRIVER_OBJECT p_driver_object)
{
	this->_p_driver_object = p_driver_object;

}

ProcessProtect::~ProcessProtect()
{
	//析构时释放回调
	if (this->_callbacks_handle)
		ObUnRegisterCallbacks(this->_callbacks_handle);

}

void* ProcessProtect::operator new(size_t size, POOL_TYPE pool_type)
{
    return ExAllocatePoolWithTag(pool_type, size, 'proc');
}

void ProcessProtect::operator delete(void* pointer)
{
    ExFreePoolWithTag(pointer, 'proc');
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

NTSTATUS ProcessProtect::RegisterProtectProcessCallbacks()
{
	NTSTATUS nt_handle_callback = STATUS_UNSUCCESSFUL;
	REG_CONTEXT reg_context;
	UNICODE_STRING us_altitude;
	OB_OPERATION_REGISTRATION ob_operation_registration[2];
	OB_CALLBACK_REGISTRATION ob_callback_registration;

	//未签名验证，解决ObRegisterCallbacks返回：0xC0000022 ->未签名
	*(PULONG)((ULONG64)this->_p_driver_object->DriverSection + 0x68) |= 0x20;

	memset(&reg_context, 0, sizeof(REG_CONTEXT));
	reg_context.ulIndex = 1;
	reg_context.Version = 120;

	RtlInitUnicodeString(&us_altitude, L"1000");

	memset(&ob_operation_registration, 0, sizeof(OB_OPERATION_REGISTRATION));
	memset(&ob_callback_registration, 0, sizeof(OB_CALLBACK_REGISTRATION));

	if ((USHORT)ObGetFilterVersion() == OB_FLT_REGISTRATION_VERSION)
	{
		ob_operation_registration[1].ObjectType = PsProcessType;
		ob_operation_registration[1].Operations = OB_OPERATION_HANDLE_CREATE | OB_OPERATION_HANDLE_DUPLICATE;
		ob_operation_registration[1].PreOperation = ProcessProtect::ProcessHandlePreCallback;
		ob_operation_registration[1].PostOperation = NULL;

		ob_operation_registration[0].ObjectType = PsThreadType;
		ob_operation_registration[0].Operations = OB_OPERATION_HANDLE_CREATE | OB_OPERATION_HANDLE_DUPLICATE;
		ob_operation_registration[0].PreOperation = ProcessProtect::ThreadHandlePreCallback;
		ob_operation_registration[0].PostOperation = NULL;

		ob_callback_registration.Version = OB_FLT_REGISTRATION_VERSION;
		ob_callback_registration.OperationRegistrationCount = 2;
		ob_callback_registration.RegistrationContext = NULL;
		ob_callback_registration.OperationRegistration = ob_operation_registration;

		//注册回调
		nt_handle_callback = ObRegisterCallbacks(&ob_callback_registration, &this->_callbacks_handle);
		if (!NT_SUCCESS(nt_handle_callback))
		{
			if (this->_callbacks_handle)
			{
				ObUnRegisterCallbacks(this->_callbacks_handle);
				this->_callbacks_handle = NULL;
			}

			DbgPrint("[BAC]:Failed to install ObRegisterCallbacks: 0x%08X.\n", nt_handle_callback);
			return STATUS_UNSUCCESSFUL;
		}
	}

	return STATUS_SUCCESS;
}

char* ProcessProtect::GetProtectProcessName()
{
	return this->_protect_process_name;
}

HANDLE ProcessProtect::GetProcessID(const char* process_name)
{
	PEPROCESS eprocess = NULL;

	for (int temp = 0; temp < 100000; temp += 4)
	{
		if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)temp, &eprocess)))
		{
			//通过eprocess获取进程名的函数需更新，否则只能获取15个字节
			if (_stricmp(PsGetProcessImageFileName(eprocess), process_name) == 0)
			{
				//DbgPrint("[BAC]:process name: %s --> id = %d\n", PsGetProcessImageFileName(eprocess), PsGetProcessId(eprocess));
				ObDereferenceObject(eprocess);
				return PsGetProcessId(eprocess);
			}
			ObDereferenceObject(eprocess);
		}
	}

	return 0;
}

bool ProcessProtect::JudgmentProtectProcess(PEPROCESS p_current_eproc)
{
	PCHAR p_current_process_name = PsGetProcessImageFileName(p_current_eproc);

	if (p_current_process_name)
	{
		if (_stricmp(this->_protect_process_name, p_current_process_name) == 0)
			return true;
		else
			return false;
	}

	return false;
}

OB_PREOP_CALLBACK_STATUS ProcessProtect::ProcessHandlePreCallback(PVOID registration_context, POB_PRE_OPERATION_INFORMATION operation_information)
{
	if (operation_information->KernelHandle)
		return OB_PREOP_SUCCESS;

	PEPROCESS opened_process = (PEPROCESS)operation_information->Object;
	PEPROCESS current_process = PsGetCurrentProcess();

	HANDLE ul_process_id = (HANDLE)PsGetProcessId(opened_process);
	HANDLE my_process_id = (HANDLE)PsGetProcessId(current_process);

	//判断是否需要保护的进程id
	HANDLE protect_pid = bac->ProcessProtect::GetProcessID("TestGame.exe");
	if (protect_pid)
	{
		if (PsGetProcessId((PEPROCESS)operation_information->Object) == protect_pid)
		{
			//striping handle
			if (operation_information->Operation == OB_OPERATION_HANDLE_CREATE)
				operation_information->Parameters->CreateHandleInformation.DesiredAccess = (SYNCHRONIZE);
			else
				operation_information->Parameters->DuplicateHandleInformation.DesiredAccess = (SYNCHRONIZE);

			return OB_PREOP_SUCCESS;
		}
	}

	return OB_PREOP_SUCCESS;
}

OB_PREOP_CALLBACK_STATUS ProcessProtect::ThreadHandlePreCallback(PVOID registration_context, POB_PRE_OPERATION_INFORMATION operation_information)
{
	if (operation_information->KernelHandle)
		return OB_PREOP_SUCCESS;

	HANDLE protect_pid = bac->ProcessProtect::GetProcessID("TestGame.exe");

	//放走进程自己线程的权限访问
	if (PsGetCurrentProcessId() == protect_pid)
		return OB_PREOP_SUCCESS;

	if (PsGetThreadProcessId((PETHREAD)operation_information->Object) == protect_pid)
	{
		if (operation_information->Operation == OB_OPERATION_HANDLE_CREATE)
			operation_information->Parameters->CreateHandleInformation.DesiredAccess = (SYNCHRONIZE | THREAD_QUERY_LIMITED_INFORMATION);
		else
			operation_information->Parameters->DuplicateHandleInformation.DesiredAccess = (SYNCHRONIZE | THREAD_QUERY_LIMITED_INFORMATION);
	}

	return OB_PREOP_SUCCESS;
}

NTSTATUS ProcessProtect::ProtectProcess(const char* process_name)
{
	//保存需要保护的进程名
	strcpy(this->_protect_process_name, process_name);

	HANDLE protect_process_id = this->GetProcessID(this->_protect_process_name);

	//注册回调
	return this->RegisterProtectProcessCallbacks();
}

