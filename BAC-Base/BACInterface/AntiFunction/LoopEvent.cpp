////////////////////////////////////////////////////////////////
//						在此处理循环事件
////////////////////////////////////////////////////////////////
#include "../BAC.h"
#include "../../BAC-Base.h"


void EventThread()
{
#if NDEBUG
	VMProtectBegin("EventThread");
#endif
#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Enter");
#endif

	//处理循环事件
	while (true)
	{
		//检查Hook是否被还原
		bac->CheckHookPointer();


		::Sleep(5000);
	}

#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
#if NDEBUG
	VMProtectEnd();
#endif
}

void BAC::LoopEvent()
{
#if NDEBUG
	VMProtectBegin("BAC::LoopEvent");
#endif
#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Enter");
#endif

	::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)EventThread, NULL, NULL, NULL);

#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
#if NDEBUG
	VMProtectEnd();
#endif
}

