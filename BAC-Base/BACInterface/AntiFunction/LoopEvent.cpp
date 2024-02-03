////////////////////////////////////////////////////////////////
//						在此处理循环事件
////////////////////////////////////////////////////////////////
#include "../BAC.h"
#include "../../BAC-Base.h"


void BAC::LoopEvent()
{
#if NDEBUG
	VMProtectBeginUltra("BAC::LoopEvent");
#endif
	baclog->FunctionLog(__FUNCTION__, "Enter");

	//处理循环事件
	while (true)
	{
		//检查Hook是否被还原
		bac->CheckHookPointer();
		//扫描内存


		::Sleep(120000);
	}

	baclog->FunctionLog(__FUNCTION__, "Leave");
#if NDEBUG
	VMProtectEnd();
#endif
}

