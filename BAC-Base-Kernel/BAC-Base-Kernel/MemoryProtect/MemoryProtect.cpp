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

NTSTATUS MemoryProtect::RemapImage(const char* module_name, HANDLE pid, DWORD64 memory_address)
{
    NTSTATUS status = STATUS_SUCCESS;

	//先判断进程是否存在
	if (!pid)
		return STATUS_UNSUCCESSFUL;

	//再判断是否需要遍历模块
	if (module_name)
	{
		wchar_t w_module_name[] = { NULL };
		this->CHAR2TCHAR(module_name, strlen(module_name), w_module_name);

	}
	





    return status;
}



