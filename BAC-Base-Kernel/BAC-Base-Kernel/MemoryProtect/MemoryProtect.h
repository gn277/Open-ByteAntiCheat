#pragma once
#include <ntifs.h>
#include <ntstrsafe.h>

typedef struct _SYSTEM_THREADS
{
	LARGE_INTEGER  KernelTime;
	LARGE_INTEGER  UserTime;
	LARGE_INTEGER  CreateTime;
	ULONG    WaitTime;
	PVOID    StartAddress;
	CLIENT_ID   ClientID;
	KPRIORITY   Priority;
	KPRIORITY   BasePriority;
	ULONG    ContextSwitchCount;
	ULONG    ThreadState;
	KWAIT_REASON  WaitReason;
	ULONG    Reserved; //Add
}SYSTEM_THREADS, * PSYSTEM_THREADS;
typedef struct _SYSTEM_PROCESSES
{
	ULONG    NextEntryDelta;//���ɽṹ���е�ƫ����
	ULONG    ThreadCount;//�߳���Ŀ
	ULONG    Reserved[6];
	LARGE_INTEGER  CreateTime;//����ʱ��;
	LARGE_INTEGER  UserTime;//�û�ģʽ(Ring3)����ʱ��
	LARGE_INTEGER  KernelTime;//�ں�ģʽ(Ring0)����ʱ��
	UNICODE_STRING  ProcessName;//������
	KPRIORITY   BasePriority;//��������Ȩ
	HANDLE   ProcessId;  //����ID
	HANDLE   InheritedFromProcessId;//������ID
	ULONG    HandleCount;//�����Ŀ
	ULONG    SessionId;
	ULONG_PTR  PageDirectoryBase;
	VM_COUNTERS VmCounters;//����Ĵ����ṹ
	SIZE_T    PrivatePageCount;//Add
	IO_COUNTERS  IoCounters; //windows 2000 only IO�������ṹ
	struct _SYSTEM_THREADS Threads[1];//��������̵߳Ľṹ����
}SYSTEM_PROCESSES, * PSYSTEM_PROCESSES;


class MemoryProtect
{
private:

private:
	NTSTATUS CHAR2TCHAR(const char* c, int len, wchar_t* p_w);
	HANDLE GetProcessIDByName(const wchar_t* process_name);

public:
	void* operator new(size_t size, POOL_TYPE pool_type = NonPagedPool);
	void operator delete(void* pointer);

public:
	MemoryProtect();
	~MemoryProtect();

public:
	NTSTATUS RemapImage(const char* module_name, HANDLE pid, DWORD64 memory_address);

};

