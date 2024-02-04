#pragma once
#include <ntifs.h>

typedef struct _OB_REG_CONTEXT
{
	USHORT Version;
	UNICODE_STRING Altitude;
	USHORT ulIndex;
	OB_OPERATION_REGISTRATION* OperationRegistration;
} REG_CONTEXT, * PREG_CONTEXT;
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

typedef struct _ProtectProcessList
{
	LIST_ENTRY list_entry;
	HANDLE protect_process_id;
}ProtectProcessList, * PProtectProcessList;


class ProcessProtect
{
private:
	PDRIVER_OBJECT _p_driver_object = nullptr;

	//需要保护进程的链表
	LIST_ENTRY _protect_process_list = { NULL };

	HANDLE _protect_pid = 0;
	wchar_t _protect_process_name[100] = { NULL };
	HANDLE _callbacks_handle = NULL;

private:
	__int64 ExpLookupHandleTableEntry(unsigned int* a1, __int64 a2);
	NTSTATUS RegisterProtectProcessCallbacks();
	//删除所有链表并释放内存
	void RemoveAllProtectProcessList();

public:
	void* operator new(size_t size, POOL_TYPE pool_type = NonPagedPool);
	void operator delete(void* pointer);

public:
	ProcessProtect(PDRIVER_OBJECT p_driver_object);
	~ProcessProtect();

public:
	wchar_t* GetProtectProcessName();
	HANDLE GetProcessID(const char* process_name);
	HANDLE GetProcessIDByName(const wchar_t* process_name);
	//多个进程情况下加入链表中
	bool GetProcessIDByNameToList(const wchar_t* process_name);
	static OB_PREOP_CALLBACK_STATUS ProcessHandlePreCallback(PVOID registration_context, POB_PRE_OPERATION_INFORMATION operation_information);
	static OB_PREOP_CALLBACK_STATUS ThreadHandlePreCallback(PVOID registration_context, POB_PRE_OPERATION_INFORMATION operation_information);

	//添加保护进程到链表
	void AddProtectProcess(const wchar_t* process_name);
	PLIST_ENTRY GetProtectProcessList();

public:
	NTSTATUS ProtectProcess(const wchar_t* process_name);

};

