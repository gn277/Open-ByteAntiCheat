////////////////////////////////////////////////////////////////
//						实现对外调用的接口
////////////////////////////////////////////////////////////////
#include "BAC-Base.h"

BAC* bac = nullptr;
BACLog* baclog = nullptr;
HMODULE self_module = NULL;


LONG CALLBACK UnHandleException(EXCEPTION_POINTERS* p_exception)
{
	bac->UnInitializeBACKernel();

	return EXCEPTION_CONTINUE_SEARCH;
}

bool BACBaseInitialize()
{
	//实例化BAC日志对象
	baclog = new BACLog();
	//实例化BAC对象
	bac = new BAC();

#if NDEBUG
	VMProtectBeginUltra("BACBaseInitialize");
#endif
	baclog->FunctionLog(__FUNCTION__, "Enter");

	//设置顶层异常过滤函数
	::SetUnhandledExceptionFilter(&UnHandleException);

	//加载驱动 如驱动未签名此处会触发异常断下，请检查驱动签名
	if (!bac->InitializeBACKernel())
		throw "initialize bac kernel error!";

	//应用层隐藏hook
	bac->HideHook();
	//监视LdrLoadDll注入
	bac->MonitorLdrLoadDll();
	//监视输入法注入
	bac->MonitorImm();
	//监视内存读写操作
	bac->MonitorMemoryOption();
	//监视窗口创建的相关函数
	bac->MonitorCreateWindow();


	//处理循环事件
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)BAC::LoopEvent, NULL, NULL, NULL);

#if NDEBUG
	VMProtectEnd();
#endif
	baclog->FileLogf("%s-> %s: %s", "[BAC]", __FUNCTION__, "Leave");
	return true;
}

bool BACBaseUnInitialize()
{
#if NDEBUG
	VMProtectBeginUltra("BACBaseUnInitialize");
#endif
	baclog->FunctionLog(__FUNCTION__, "Enter");

	//释放BAC对象
	if (bac)
	{
		if(!bac->UnInitializeBACKernel())
			throw "uninitialize bac kernel error";
		delete bac;
	}

	baclog->FunctionLog(__FUNCTION__, "Leave");
#if NDEBUG
	VMProtectEnd();
#endif

	//释放BAC日志对象
	if (baclog)
		delete baclog;

	return true;
}

BOOL APIENTRY DllMain(HMODULE h_module, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			self_module = h_module;

			if (!BACBaseInitialize())
				MessageBoxA(NULL, "BAC load error, please check!", "Error", MB_OK);

			break;
		}
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
		{
			BACBaseUnInitialize();
			break;
		}
	}
	return TRUE;
}

