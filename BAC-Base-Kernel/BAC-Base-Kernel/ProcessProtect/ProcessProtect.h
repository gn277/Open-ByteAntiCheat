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

typedef struct _ProtectProcessList
{
	LIST_ENTRY list_entry;
	HANDLE protect_process_id;
}ProtectProcessList, * PProtectProcessList;


class ProcessProtect
{
private:
	PDRIVER_OBJECT _p_driver_object = nullptr;

	//��Ҫ�������̵�����
	LIST_ENTRY _protect_process_list = { NULL };

	HANDLE _protect_pid = 0;
	wchar_t _protect_process_name[100] = { NULL };
	HANDLE _callbacks_handle = NULL;

private:
	__int64 ExpLookupHandleTableEntry(unsigned int* a1, __int64 a2);
	NTSTATUS RegisterProtectProcessCallbacks();
	//ɾ�����������ͷ��ڴ�
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
	//�����������¼���������
	bool GetProcessIDByNameToList(const wchar_t* process_name);
	static OB_PREOP_CALLBACK_STATUS ProcessHandlePreCallback(PVOID registration_context, POB_PRE_OPERATION_INFORMATION operation_information);
	static OB_PREOP_CALLBACK_STATUS ThreadHandlePreCallback(PVOID registration_context, POB_PRE_OPERATION_INFORMATION operation_information);

	//��ӱ������̵�����
	void AddProtectProcess(const wchar_t* process_name);
	PLIST_ENTRY GetProtectProcessList();

public:
	NTSTATUS ProtectProcess(const wchar_t* process_name);

};

