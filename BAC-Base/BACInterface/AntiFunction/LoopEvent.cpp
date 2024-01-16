////////////////////////////////////////////////////////////////
//						在此处理循环事件
////////////////////////////////////////////////////////////////
#include "../BAC.h"
#include "../../BAC-Base.h"


void BAC::LoopEvent()
{
#if _DEBUG
	baclog->OutPutCommandLine(__FUNCTION__, "Initialize");
#endif



#if _DEBUG
	baclog->OutPutCommandLine(__FUNCTION__, "Leave");
#endif	
}

