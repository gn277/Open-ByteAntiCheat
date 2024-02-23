#include "../BAC-Base-Kernel.h"
#include "../../DriverEntry.h"


void BACBase::KernelLoopEvent(IN IN PVOID start_context)
{
#if NDEBUG
	VMProtectBeginUltra("BACBase::KernelLoopEvent");
#endif
#if _DEBUG
	OutPutBACLog(__FUNCTION__, "Enter");
#endif

	while (true)
	{
		if (!bac->GetKernelThreadStatus())
			break;

		//�������������б�
		PLIST_ENTRY protect_process_list = bac->ProcessProtect::GetProtectProcessList();
		if (!IsListEmpty(protect_process_list))
		{
			PLIST_ENTRY p_current_list = protect_process_list->Flink;
			while (p_current_list != protect_process_list)
			{
				PProtectProcessList node = (PProtectProcessList)p_current_list;
				if (node->protect_process_id)
				{
					DbgPrint("[BAC]:֪ͨӦ�ò���ȡ�ں�����\n");

					//���̴�����֪ͨӦ�ò�ȡ����
					KeSetEvent(bac->GetFileEventHandle(), 0, FALSE);
				}
				else
					DbgPrint("[BAC]:BACBase::KernelLoopEvent-> �����б�Ϊ��\n");

				p_current_list = p_current_list->Flink;
			}
		}

		//ѭ��
		bac->BACTools::Sleep(KERNEL_THREAD_SLEEP_TIME);
	}

	PsTerminateSystemThread(STATUS_SUCCESS);
#if _DEBUG
	OutPutBACLog(__FUNCTION__, "Leave");
#endif
#if NDEBUG
	VMProtectEnd();
#endif
}

void BACBase::KernelIoTimerEvent(PDEVICE_OBJECT device_object, PVOID context)
{
#if NDEBUG
	VMProtectBeginUltra("BACBase::KernelIoTimerEvent");
#endif

	//���������б�
	PLIST_ENTRY protect_process_list = bac->ProcessProtect::GetProtectProcessList();
	if (!IsListEmpty(protect_process_list))
	{
		PLIST_ENTRY p_current_list = protect_process_list->Flink;
		while (p_current_list != protect_process_list)
		{
			PProtectProcessList node = (PProtectProcessList)p_current_list;
			if (node->protect_process_id)
			{
				DbgPrint("[BAC]:io timer\n");

			}
			else
				DbgPrint("[BAC]:BACBase::KernelIoTimerEvent-> �����б�Ϊ��\n");

			p_current_list = p_current_list->Flink;
		}
	}

#if NDEBUG
	VMProtectEnd();
#endif
}

NTSTATUS BACBase::InitializeKernelLoopEvent(IN HANDLE file_handle)
{
#if _DEBUG
	OutPutBACLog(__FUNCTION__, "Enter");
#endif

	NTSTATUS status = STATUS_SUCCESS;

	status = ObReferenceObjectByHandle(file_handle, EVENT_MODIFY_STATE, *ExEventObjectType,
		KernelMode, (PVOID*)&this->_file_event_handle, NULL);
	if (NT_SUCCESS(status))
	{
		//�����ں�ѭ���¼��߳�
		HANDLE thread_handle = NULL;
		status = PsCreateSystemThread(&thread_handle, 0, NULL, NULL, &this->_kernel_loop_event_handle, BACBase::KernelLoopEvent, NULL);
	}

#if _DEBUG
	OutPutBACLog(__FUNCTION__, "Leave");
#endif
	return status;
}

NTSTATUS BACBase::InitializeIoTimer(PDEVICE_OBJECT p_device)
{
#if _DEBUG
	OutPutBACLog(__FUNCTION__, "Enter");
#endif

	NTSTATUS status = STATUS_SUCCESS;

	//IO��ʱ��
	status = IoInitializeTimer(p_device, BACBase::KernelIoTimerEvent, NULL);
	IoStartTimer(p_device);

#if _DEBUG
	OutPutBACLog(__FUNCTION__, "Leave");
#endif
	return status;
}

