#include "ProcessProtect.h"
#include "../../DriverEntry.h"

//申明函数
//extern "C" PCHAR PsGetProcessImageFileName(IN PEPROCESS p_process);
//extern "C" NTSYSAPI NTSTATUS NTAPI ZwQueryInformationProcess(
//	IN HANDLE ProcessHandle,
//	IN PROCESSINFOCLASS ProcessInformationClass,
//	OUT PVOID ProcessInformation,
//	IN ULONG ProcessInformationLength,
//	IN PULONG ReturnLength);
//extern "C" NTSYSAPI NTSTATUS NTAPI ZwQuerySystemInformation(
//	IN ULONG SystemInformationClass,
//	OUT PVOID SystemInformation,
//	IN ULONG SystemInformationLength,
//	OUT PULONG ReturnLength);


ProcessProtect::ProcessProtect(PDRIVER_OBJECT p_driver_object)
{
	this->_p_driver_object = p_driver_object;

	//初始化链表头
	InitializeListHead(&this->_protect_process_list);

}

ProcessProtect::~ProcessProtect()
{
	//析构时释放回调
	if (this->_callbacks_handle)
		ObUnRegisterCallbacks(this->_callbacks_handle);

	//释放链表
	this->RemoveAllProtectProcessList();

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

void ProcessProtect::RemoveAllProtectProcessList()
{
	PLIST_ENTRY p_list = nullptr;

	while (!IsListEmpty(&this->_protect_process_list))
	{
		//移除链表
		p_list = RemoveHeadList(&this->_protect_process_list);

		//释放内存
		ExFreePool((PProtectProcessList)p_list);
	}
}

wchar_t* ProcessProtect::GetProtectProcessName()
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
				DbgPrint("[BAC]:process name: %s --> id = %d\n", PsGetProcessImageFileName(eprocess), PsGetProcessId(eprocess));
				ObDereferenceObject(eprocess);
				return PsGetProcessId(eprocess);
			}
			ObDereferenceObject(eprocess);
		}
	}

	return 0;
}

HANDLE ProcessProtect::GetProcessIDByName(const wchar_t* process_name)
{
	NTSTATUS status = STATUS_SUCCESS;
	ULONG ret_length = 0;
	PVOID p_proc_info = 0;
	PSYSTEM_PROCESSES p_proc_index = { NULL };

	//调用函数，获取进程信息
	status = ZwQuerySystemInformation(5/*获取进程信息,宏定义为5*/, NULL, 0, &ret_length);
	if (!ret_length)
	{
		DbgPrint("[BAC]:ZwQuerySystemInformation error!\n");
		return 0;
	}

	//申请空间
	p_proc_info = ExAllocatePool(NonPagedPool, ret_length);
	if (!p_proc_info)
	{
		DbgPrint("[BAC]:ExAllocatePool error!\n");
		return 0;
	}

	status = ZwQuerySystemInformation(5/*获取进程信息,宏定义为5*/, p_proc_info, ret_length, &ret_length);
	if (NT_SUCCESS(status))
	{
		p_proc_index = (PSYSTEM_PROCESSES)p_proc_info;

		////第一个进程应该是 pid 为 0 的进程
		//if (p_proc_index->ProcessId == 0)
		//	DbgPrint("[BAC]:PID 0 System Idle Process\n");

		//循环打印所有进程信息,因为最后一个进程的NextEntryDelta值为0,所以先打印后判断
		do
		{
			p_proc_index = (PSYSTEM_PROCESSES)((char*)p_proc_index + p_proc_index->NextEntryDelta);

			if (_wcsicmp(p_proc_index->ProcessName.Buffer, process_name) == 0)
			{
				HANDLE pid = p_proc_index->ProcessId;
				//DbgPrint("[BAC]:ProcName:  %-20ws     pid:  %u\n", p_proc_index->ProcessName.Buffer, pid);

				ExFreePool(p_proc_info);
				return pid;
			}
		} while (p_proc_index->NextEntryDelta != 0);
	}
	//else
	//	DbgPrint("[BAC]:GetProcessIDByName-> error code : %u!!!\n", status);

	ExFreePool(p_proc_info);

	return 0;
}

bool ProcessProtect::GetProcessIDByNameToList(const wchar_t* process_name)
{
	NTSTATUS status = STATUS_SUCCESS;
	ULONG ret_length = 0;
	PVOID p_proc_info = 0;
	PSYSTEM_PROCESSES p_proc_index = { NULL };

	//调用函数，获取进程信息
	status = ZwQuerySystemInformation(5/*获取进程信息,宏定义为5*/, NULL, 0, &ret_length);
	if (!ret_length)
	{
		DbgPrint("[BAC]:ZwQuerySystemInformation error!\n");
		return 0;
	}

	//申请空间
	p_proc_info = ExAllocatePool(NonPagedPool, ret_length);
	if (!p_proc_info)
	{
		DbgPrint("[BAC]:ExAllocatePool error!\n");
		return 0;
	}

	status = ZwQuerySystemInformation(5/*获取进程信息,宏定义为5*/, p_proc_info, ret_length, &ret_length);
	if (NT_SUCCESS(status))
	{
		p_proc_index = (PSYSTEM_PROCESSES)p_proc_info;

		//循环打印所有进程信息,因为最后一个进程的NextEntryDelta值为0,所以先打印后判断
		do
		{
			p_proc_index = (PSYSTEM_PROCESSES)((char*)p_proc_index + p_proc_index->NextEntryDelta);

			if (_wcsicmp(p_proc_index->ProcessName.Buffer, process_name) == 0)
			{
				HANDLE pid = p_proc_index->ProcessId;
				//DbgPrint("[BAC]:ProcName:  %-20ws     pid:  %u\n", p_proc_index->ProcessName.Buffer, pid);

				PProtectProcessList p_list = (PProtectProcessList)ExAllocatePoolWithTag(NonPagedPool, sizeof(ProtectProcessList), 'list');
				if (!p_list)
					return false;

				//保存进程id
				p_list->protect_process_id = pid;
				//添加到保存链表中
				//InsertHeadList(&this->_protect_process_list, (PLIST_ENTRY)p_list);
				InsertTailList(&this->_protect_process_list, &p_list->list_entry);
			}
		} while (p_proc_index->NextEntryDelta != 0);
	}
	else
		return false;

	ExFreePool(p_proc_info);

	return true;
}

OB_PREOP_CALLBACK_STATUS ProcessProtect::ProcessHandlePreCallback(PVOID registration_context, POB_PRE_OPERATION_INFORMATION operation_information)
{
	if (operation_information->KernelHandle)
		return OB_PREOP_SUCCESS;

	PEPROCESS opened_process = (PEPROCESS)operation_information->Object;
	PEPROCESS current_process = PsGetCurrentProcess();
	//HANDLE ul_process_id = (HANDLE)PsGetProcessId(opened_process);
	//HANDLE my_process_id = (HANDLE)PsGetProcessId(current_process);
	
	//遍历保护进程列表
	PLIST_ENTRY protect_process_list = bac->ProcessProtect::GetProtectProcessList();
	if (!IsListEmpty(protect_process_list))
	{
		PLIST_ENTRY p_current_list = protect_process_list->Flink;
		while (p_current_list != protect_process_list)
		{
			PProtectProcessList node = (PProtectProcessList)p_current_list;
			if (PsGetProcessId((PEPROCESS)operation_information->Object) == node->protect_process_id)
			{
				//striping handle
				if (operation_information->Operation == OB_OPERATION_HANDLE_CREATE)
					operation_information->Parameters->CreateHandleInformation.DesiredAccess = (SYNCHRONIZE);
				else
					operation_information->Parameters->DuplicateHandleInformation.DesiredAccess = (SYNCHRONIZE);
			
				return OB_PREOP_SUCCESS;
			}
	
			p_current_list = p_current_list->Flink;
		}
	}

	return OB_PREOP_SUCCESS;
}

OB_PREOP_CALLBACK_STATUS ProcessProtect::ThreadHandlePreCallback(PVOID registration_context, POB_PRE_OPERATION_INFORMATION operation_information)
{
	if (operation_information->KernelHandle)
		return OB_PREOP_SUCCESS;

	//遍历保护进程列表
	PLIST_ENTRY protect_process_list = bac->ProcessProtect::GetProtectProcessList();
	if (!IsListEmpty(protect_process_list))
	{
		PLIST_ENTRY p_current_list = protect_process_list->Flink;
		while (p_current_list != protect_process_list)
		{
			PProtectProcessList node = (PProtectProcessList)p_current_list;

			//放走进程自己线程的权限访问
			if (PsGetCurrentProcessId() == node->protect_process_id)
				return OB_PREOP_SUCCESS;

			if (PsGetProcessId((PEPROCESS)operation_information->Object) == node->protect_process_id)
			{
				//striping handle
				if (operation_information->Operation == OB_OPERATION_HANDLE_CREATE)
					operation_information->Parameters->CreateHandleInformation.DesiredAccess = (SYNCHRONIZE | THREAD_QUERY_LIMITED_INFORMATION);
				else
					operation_information->Parameters->DuplicateHandleInformation.DesiredAccess = (SYNCHRONIZE | THREAD_QUERY_LIMITED_INFORMATION);

				return OB_PREOP_SUCCESS;
			}

			p_current_list = p_current_list->Flink;
		}
	}

	return OB_PREOP_SUCCESS;
}

void ProcessProtect::AddProtectProcess(const wchar_t* process_name)
{

}

PLIST_ENTRY ProcessProtect::GetProtectProcessList()
{
	return &this->_protect_process_list;
}

NTSTATUS ProcessProtect::ProtectProcess(const wchar_t* process_name)
{
	//保存需要保护的进程名
	wcscpy(this->_protect_process_name, process_name);

	//DbgPrint("[BAC]:需要保护的进程：%S", this->_protect_process_name);
	this->_protect_pid = this->GetProcessIDByName(this->_protect_process_name);

	//遍历所有同名的进程
	if (!this->GetProcessIDByNameToList(process_name))
	{
		DbgPrint("[BAC]:GetProcessIDByNameToList error!");
		return STATUS_UNSUCCESSFUL;
	}

	//注册回调
	return this->RegisterProtectProcessCallbacks();
}

NTSTATUS ProcessProtect::ClearDebugPort(IN HANDLE pid)
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	PEPROCESS p_eprocess = nullptr;
	PVOID p_debug_object = nullptr;

	if (!pid)
		return status;

	status = PsLookupProcessByProcessId(pid, &p_eprocess);
	if (!NT_SUCCESS(status) || !p_eprocess)
		return status;
	ObDereferenceObject(p_eprocess);

	p_debug_object = PsGetProcessDebugPort(p_eprocess);
	if (p_debug_object)
	{
		//剥离内核调试器，没有返回STATUS_DEBUGGER_INACTIVE，内核调试器处于阻塞状态返回STATUS_ACCESS_DENIED，成功返回STATUS_SUCCESS
		KdDisableDebugger();

		if (MmIsAddressValid((ULONG*)((PUCHAR)p_eprocess + WIN11_X64_DEBUGPORT_OFFSET)))
		{
			//发现调试器后可以进行剥离处理，这里直接不剥离置零后让系统崩溃
			*(ULONG*)*(ULONG*)((PUCHAR)p_eprocess + WIN11_X64_DEBUGPORT_OFFSET) = 0;
		}

		status = STATUS_SUCCESS;
	}

	return status;
}

