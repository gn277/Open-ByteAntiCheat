////////////////////////////////////////////////////////////////
//						在此处理循环事件
////////////////////////////////////////////////////////////////
#include "../BAC.h"


void BAC::InitializeLoopEnvent()
{
	baclog->FunctionLog(__FUNCTION__, "Enter");



	baclog->FunctionLog(__FUNCTION__, "Leave");
}

void BAC::LoopEvent()
{
#if NDEBUG
	VMProtectBeginUltra("BAC::LoopEvent");
#endif
	baclog->FunctionLog(__FUNCTION__, "Enter");

	//做一些初始化处理后再处理事件
	bac->InitializeLoopEnvent();

	//处理循环事件
	while (true)
	{
		//检查Hook是否被还原
		bac->CheckHookPointer();
		//扫描进程内存CRC32
		bac->CheckMemoryCRC32();
#if NDEBUG
		//清空游戏进程的调试端口
		if (!bac->BACKernel::ClearProcessDebugPort((HANDLE)::GetCurrentProcessId()))
		{
			MessageBoxA(::GetActiveWindow(), "clear process debug port error", "BAC::Error", MB_OK);
			ExitProcess(-1);
		}
#endif

		::Sleep(120000);
	}

	baclog->FunctionLog(__FUNCTION__, "Leave");
#if NDEBUG
	VMProtectEnd();
#endif
}

