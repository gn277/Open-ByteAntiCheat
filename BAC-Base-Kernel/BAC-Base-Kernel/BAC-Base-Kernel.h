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

//内核线程循环等待时间
#define KERNEL_THREAD_SLEEP_TIME 1000

//内核消息队列结构
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
	//应用层文件读写句柄(用于内核的文件读写通讯)
	PKEVENT _file_event_handle = NULL;

	//内核循环线程运行状态
	bool _kernel_loop_event_status = true;
	//内核层循环事件线程句柄
	CLIENT_ID _kernel_loop_event_handle = { NULL };

	//内核消息派遣

public:
	BACBase(PDRIVER_OBJECT p_driver_object);
	~BACBase();

public:
	void* operator new(size_t size, POOL_TYPE pool_type = NonPagedPool);
	void operator delete(void* pointer);

	//设置应用层文件读写句柄
	void SetFileEventHandle(PKEVENT file_hanle);
	//获取应用层文件读写句柄
	PKEVENT GetFileEventHandle();
	//设置内核线程运行状态
	void SetKernelThreadStatus(bool status);
	//获取内核线程运行状态
	bool GetKernelThreadStatus();

public:
	//内核循环事件
	static void KernelLoopEvent(IN IN PVOID start_context);
	//初始化内核计时器循环函数
	static void KernelIoTimerEvent(PDEVICE_OBJECT device_object, PVOID context);

public:
	//初始化内核循环事件
	NTSTATUS InitializeKernelLoopEvent(IN HANDLE file_handle);
	//初始化内核计时器
	NTSTATUS InitializeIoTimer(PDEVICE_OBJECT p_device);

};

