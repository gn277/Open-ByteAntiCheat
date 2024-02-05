#include "MemoryProtect.h"

extern "C" NTSYSAPI NTSTATUS NTAPI ZwQueryInformationProcess(
	IN HANDLE ProcessHandle,
	IN PROCESSINFOCLASS ProcessInformationClass,
	OUT PVOID ProcessInformation,
	IN ULONG ProcessInformationLength,
	IN PULONG ReturnLength);
extern "C" NTSYSAPI NTSTATUS NTAPI ZwQuerySystemInformation(
	IN ULONG SystemInformationClass,
	OUT PVOID SystemInformation,
	IN ULONG SystemInformationLength,
	OUT PULONG ReturnLength);


MemoryProtect::MemoryProtect()
{
}

MemoryProtect::~MemoryProtect()
{
}

void* MemoryProtect::operator new(size_t size, POOL_TYPE pool_type)
{
    return ExAllocatePoolWithTag(pool_type, size, 'memo');
}

void MemoryProtect::operator delete(void* pointer)
{
    ExFreePoolWithTag(pointer, 'memo');
}

NTSTATUS MemoryProtect::CHAR2TCHAR(const char* c, int len, wchar_t* p_w)
{
    return RtlStringCbPrintfW(p_w, (len + 1) * 2, L"%hs", c);
}

HANDLE MemoryProtect::GetProcessIDByName(const wchar_t* process_name)
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

NTSTATUS MemoryProtect::RemapImage(const char* module_name, HANDLE pid, DWORD64 memory_address)
{
    NTSTATUS status = STATUS_SUCCESS;

	//���жϽ����Ƿ����
	if (!pid)
		return STATUS_UNSUCCESSFUL;

	//���ж��Ƿ���Ҫ����ģ��
	if (module_name)
	{
		wchar_t w_module_name[] = { NULL };
		this->CHAR2TCHAR(module_name, strlen(module_name), w_module_name);

	}
	





    return status;
}



