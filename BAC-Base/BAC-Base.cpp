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
#if NDEBUG
	VMProtectBegin("BACBaseInitialize");
#endif

	//实例化BAC日志对象
	baclog = new BACLog();
	//实例化BAC对象
	bac = new BAC();

#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Enter");
#endif

	//设置顶层异常过滤函数
	::SetUnhandledExceptionFilter(&UnHandleException);

	//加载驱动 如驱动未签名此处会触发异常断下，请检查驱动签名
	if (!bac->InitializeBACKernel())
		throw "initialize bac kernel error!";

	//Hook前测试CRC32
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

	//Hook后测试CRC32
	printf("after hook hash:%d\n", bac->CRC32(::GetModuleHandleA("ntdll.dll"), 0x10000));

#if _DEBUG
	baclog->FileLogf("%s-> %s: %s", "[BAC]", __FUNCTION__, "Leave");
#endif
	return true;
#if NDEBUG
	VMProtectEnd();
#endif
}

bool BACBaseUnInitialize()
{
#if NDEBUG
	VMProtectBegin("BACBaseUnInitialize");
#endif
#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Enter");
#endif

	//释放BAC对象
	if (bac)
	{
		if(!bac->UnInitializeBACKernel())
			throw "uninitialize bac kernel error";
		delete bac;
	}
	else
		return false;

#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Leave");
#endif

	//释放BAC日志对象
	if (baclog)
		delete baclog;
	else
		return false;
	return true;
#if NDEBUG
	VMProtectEnd();
#endif
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

