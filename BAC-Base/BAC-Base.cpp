////////////////////////////////////////////////////////////////
//						实现对外调用的接口
////////////////////////////////////////////////////////////////
#include "BAC-Base.h"

BAC* bac = nullptr;
BACLog* baclog = nullptr;


bool BACBaseInitialize()
{
	//实例化BAC日志对象
	baclog = new BACLog();
	//实例化BAC对象
	bac = new BAC();

	//测试CRC32
	printf("old hash:%d\n", bac->CRC32(::GetModuleHandleA("ntdll.dll"), 0x10000));

	//应用层隐藏hook
	bac->HideHook();
	//监视LdrLoadDll注入
	bac->MonitorLdrLoadDll();
	//监视输入法注入
	bac->MonitorImm();
	//监视内存读写操作
	bac->MonitorMemoryOption();
	//处理循环事件
	bac->LoopEvent();



	//监视窗口创建的相关函数
	bac->MonitorCreateWindow();

	//测试CRC32
	printf("after hook hash:%d\n", bac->CRC32(::GetModuleHandleA("ntdll.dll"), 0x10000));

	printf("BAC Initialize Over...\n");
	return true;
}

bool BACBaseUnInitialize()
{
	//释放BAC对象
	if (bac)
		delete bac;
	//释放BAC日志对象
	if (baclog)
		delete baclog;

	return true;
}

