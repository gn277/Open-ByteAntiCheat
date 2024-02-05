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
	ULONG    NextEntryDelta;//构成结构序列的偏移量
	ULONG    ThreadCount;//线程数目
	ULONG    Reserved[6];
	LARGE_INTEGER  CreateTime;//创建时间;
	LARGE_INTEGER  UserTime;//用户模式(Ring3)创建时间
	LARGE_INTEGER  KernelTime;//内核模式(Ring0)创建时间
	UNICODE_STRING  ProcessName;//进程名
	KPRIORITY   BasePriority;//进程优先权
	HANDLE   ProcessId;  //进程ID
	HANDLE   InheritedFromProcessId;//父进程ID
	ULONG    HandleCount;//句柄数目
	ULONG    SessionId;
	ULONG_PTR  PageDirectoryBase;
	VM_COUNTERS VmCounters;//虚拟寄存器结构
	SIZE_T    PrivatePageCount;//Add
	IO_COUNTERS  IoCounters; //windows 2000 only IO计数器结构
	struct _SYSTEM_THREADS Threads[1];//进程相关线程的结构数组
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

