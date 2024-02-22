#include "ProcessProtect.h"
#include "../../DriverEntry.h"

//��������
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

	//��ʼ������ͷ
	InitializeListHead(&this->_protect_process_list);

}

ProcessProtect::~ProcessProtect()
{
	//����ʱ�ͷŻص�
	if (this->_callbacks_handle)
		ObUnRegisterCallbacks(this->_callbacks_handle);

	//�ͷ�����
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

		//ע��ص�
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
		//�Ƴ�����
		p_list = RemoveHeadList(&this->_protect_process_list);

		//�ͷ��ڴ�
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
			//ͨ��eprocess��ȡ�������ĺ�������£�����ֻ�ܻ�ȡ15���ֽ�
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

	//���ú�������ȡ������Ϣ
	status = ZwQuerySystemInformation(5/*��ȡ������Ϣ,�궨��Ϊ5*/, NULL, 0, &ret_length);
	if (!ret_length)
	{
		DbgPrint("[BAC]:ZwQuerySystemInformation error!\n");
		return 0;
	}

	//����ռ�
	p_proc_info = ExAllocatePool(NonPagedPool, ret_length);
	if (!p_proc_info)
	{
		DbgPrint("[BAC]:ExAllocatePool error!\n");
		return 0;
	}

	status = ZwQuerySystemInformation(5/*��ȡ������Ϣ,�궨��Ϊ5*/, p_proc_info, ret_length, &ret_length);
	if (NT_SUCCESS(status))
	{
		p_proc_index = (PSYSTEM_PROCESSES)p_proc_info;

		////��һ������Ӧ���� pid Ϊ 0 �Ľ���
		//if (p_proc_index->ProcessId == 0)
		//	DbgPrint("[BAC]:PID 0 System Idle Process\n");

		//ѭ����ӡ���н�����Ϣ,��Ϊ���һ�����̵�NextEntryDeltaֵΪ0,�����ȴ�ӡ���ж�
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

	//���ú�������ȡ������Ϣ
	status = ZwQuerySystemInformation(5/*��ȡ������Ϣ,�궨��Ϊ5*/, NULL, 0, &ret_length);
	if (!ret_length)
	{
		DbgPrint("[BAC]:ZwQuerySystemInformation error!\n");
		return 0;
	}

	//����ռ�
	p_proc_info = ExAllocatePool(NonPagedPool, ret_length);
	if (!p_proc_info)
	{
		DbgPrint("[BAC]:ExAllocatePool error!\n");
		return 0;
	}

	status = ZwQuerySystemInformation(5/*��ȡ������Ϣ,�궨��Ϊ5*/, p_proc_info, ret_length, &ret_length);
	if (NT_SUCCESS(status))
	{
		p_proc_index = (PSYSTEM_PROCESSES)p_proc_info;

		//ѭ����ӡ���н�����Ϣ,��Ϊ���һ�����̵�NextEntryDeltaֵΪ0,�����ȴ�ӡ���ж�
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

				//�������id
				p_list->protect_process_id = pid;
				//��ӵ�����������
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
	
	//�������������б�
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

	//�������������б�
	PLIST_ENTRY protect_process_list = bac->ProcessProtect::GetProtectProcessList();
	if (!IsListEmpty(protect_process_list))
	{
		PLIST_ENTRY p_current_list = protect_process_list->Flink;
		while (p_current_list != protect_process_list)
		{
			PProtectProcessList node = (PProtectProcessList)p_current_list;

			//���߽����Լ��̵߳�Ȩ�޷���
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
	//������Ҫ�����Ľ�����
	wcscpy(this->_protect_process_name, process_name);

	//DbgPrint("[BAC]:��Ҫ�����Ľ��̣�%S", this->_protect_process_name);
	this->_protect_pid = this->GetProcessIDByName(this->_protect_process_name);

	//��������ͬ���Ľ���
	if (!this->GetProcessIDByNameToList(process_name))
	{
		DbgPrint("[BAC]:GetProcessIDByNameToList error!");
		return STATUS_UNSUCCESSFUL;
	}

	//ע��ص�
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
		//�����ں˵�������û�з���STATUS_DEBUGGER_INACTIVE���ں˵�������������״̬����STATUS_ACCESS_DENIED���ɹ�����STATUS_SUCCESS
		KdDisableDebugger();

		if (MmIsAddressValid((ULONG*)((PUCHAR)p_eprocess + WIN11_X64_DEBUGPORT_OFFSET)))
		{
			//���ֵ���������Խ��а��봦������ֱ�Ӳ������������ϵͳ����
			*(ULONG*)*(ULONG*)((PUCHAR)p_eprocess + WIN11_X64_DEBUGPORT_OFFSET) = 0;
		}

		status = STATUS_SUCCESS;
	}

	return status;
}

