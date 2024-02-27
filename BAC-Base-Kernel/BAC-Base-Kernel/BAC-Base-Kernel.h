#pragma once
#include <ntifs.h>
#include <ntdef.h>
#include <ntddk.h>
#include <windef.h>
#include <ntimage.h>
#include <wdf.h>
#include <ntstrsafe.h>

#pragma comment(lib, "NtStrSafe.lib")
#pragma comment(lib, "NtosKrnl.lib")

#include "../../BAC-Base/ProtectSDK/VMProtectSDK/VMProtectDDK.h"

#include "UnDocumentedApi.h"
#include "BACTools/BACTools.h"
#include "ProcessProtect/ProcessProtect.h"
#include "MemoryProtect/MemoryProtect.h"

//�ں��߳�ѭ���ȴ�ʱ��
#define KERNEL_THREAD_SLEEP_TIME 1000

//�ں���Ϣ���нṹ
typedef struct _MessageQueue
{
	LIST_ENTRY list_entry;
	bool sended = false;
	int buffer_len;
	PVOID buffer;
};


void OutPutBACLog(IN const char* function, IN const char* data);

class BACBase : public BACTools, public ProcessProtect, public MemoryProtect
{
private:
	//Ӧ�ò��ļ���д���(�����ں˵��ļ���дͨѶ)
	PKEVENT _file_event_handle = NULL;

	//�ں�ѭ���߳�����״̬
	bool _kernel_loop_event_status = true;
	//�ں˲�ѭ���¼��߳̾��
	CLIENT_ID _kernel_loop_event_handle = { NULL };

	//�ں���Ϣ��ǲ

public:
	BACBase(PDRIVER_OBJECT p_driver_object);
	~BACBase();

public:
	void* operator new(size_t size, POOL_TYPE pool_type = NonPagedPool);
	void operator delete(void* pointer);

	//����Ӧ�ò��ļ���д���
	void SetFileEventHandle(PKEVENT file_hanle);
	//��ȡӦ�ò��ļ���д���
	PKEVENT GetFileEventHandle();
	//�����ں��߳�����״̬
	void SetKernelThreadStatus(bool status);
	//��ȡ�ں��߳�����״̬
	bool GetKernelThreadStatus();

public:
	//�ں�ѭ���¼�
	static void KernelLoopEvent(IN IN PVOID start_context);
	//��ʼ���ں˼�ʱ��ѭ������
	static void KernelIoTimerEvent(PDEVICE_OBJECT device_object, PVOID context);

public:
	//��ʼ���ں�ѭ���¼�
	NTSTATUS InitializeKernelLoopEvent(IN HANDLE file_handle);
	//��ʼ���ں˼�ʱ��
	NTSTATUS InitializeIoTimer(PDEVICE_OBJECT p_device);

};

