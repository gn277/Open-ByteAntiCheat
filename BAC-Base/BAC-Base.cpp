////////////////////////////////////////////////////////////////
//						实现对外调用的接口
////////////////////////////////////////////////////////////////
#include "BAC-Base.h"

BAC* bac = nullptr;
BACLog* baclog = nullptr;
HMODULE self_module = NULL;


LONG CALLBACK UnHandleException(EXCEPTION_POINTERS* p_exception)
{
	bac->BACKernel::UnInstallDriver();

	return EXCEPTION_CONTINUE_SEARCH;
}

bool BACBaseInitialize()
{
#if _DEBUG
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
#endif

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

	TCHAR driver_name[MAX_PATH] = { NULL };
	TCHAR module_path[MAX_PATH] = { NULL };
	TCHAR process_path[MAX_PATH] = { NULL };
	TCHAR process_name[MAX_PATH] = { NULL };
	TCHAR driver_full_path[MAX_PATH] = { NULL };

	//获取BAC-Base模块完整路径
	GetModuleFileNameW(self_module, module_path, _countof(module_path));
	//获取驱动路径
	PathRemoveFileSpecW(module_path);
	_stprintf_s(driver_full_path, _countof(driver_full_path), _T("%s\\%s"), module_path, DRIVER_FILE_NAME);
	//获取进程名
	GetModuleFileNameW(::GetModuleHandleW(NULL), process_path, _countof(process_path));
	wcscpy(process_name, wcsrchr(process_path, '\\') + 1);
	//获取驱动文件名称
	wcscpy_s(driver_name, _countof(driver_name), driver_full_path);
	PathStripPathW(driver_name);

	//加载驱动 如驱动未签名此处会触发异常断下，请检查驱动签名
	if (!bac->BACKernel::InstiallDriver(driver_name, driver_full_path))
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
	//


	////内存操作完成后映射内存达到代码不被修改
	//if (!bac->RemapImage((ULONG_PTR)self_module))
	//	MessageBoxA(NULL, "BAC Initialize error!", "ERROR", MB_OK);
	//
	//MODULEINFO module_info = { NULL };
	//::GetModuleInformation(::GetCurrentProcess(), self_module, &module_info, sizeof(module_info));
	//if (!bac->BACKernel::RemapImage("BAC-Base64.dll", (HANDLE)::GetCurrentProcessId(), (DWORD64)self_module, module_info.SizeOfImage))
	//	MessageBoxA(NULL, "BAC Initialize error!", "ERROR", MB_OK);

#if NDEBUG
	//清空游戏进程的调试端口
	if (!bac->BACKernel::ClearProcessDebugPort((HANDLE)::GetCurrentProcessId()))
	{
		MessageBoxA(::GetActiveWindow(), "clear process debug port error", "BAC::Error", MB_OK);
		ExitProcess(-1);
	}
	//内核保护进程
	if (!bac->BACKernel::ProtectProcessByName(process_name))
	{
		if (MessageBoxA(::GetActiveWindow(), "driver load error,please check!", "BAC:Error", MB_OK))
			ExitProcess(-1);
		else
			ExitProcess(-2);
	}
#endif

	//处理循环事件
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)BAC::LoopEvent, NULL, NULL, NULL);

	baclog->FileLogf("%s-> %s: %s", "[BAC]", __FUNCTION__, "Leave");
	return true;

#if NDEBUG
	VMProtectEnd();
#endif
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
		if(!bac->BACKernel::UnInstallDriver())
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
			//::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)BACBaseInitialize, NULL, NULL, NULL);

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

