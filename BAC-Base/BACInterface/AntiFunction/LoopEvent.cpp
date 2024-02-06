////////////////////////////////////////////////////////////////
//						�ڴ˴���ѭ���¼�
////////////////////////////////////////////////////////////////
#include "../BAC.h"
#include "../../BAC-Base.h"


void BAC::LoopEvent()
{
#if NDEBUG
	VMProtectBeginUltra("BAC::LoopEvent");
#endif
	baclog->FunctionLog(__FUNCTION__, "Enter");

	//����ѭ���¼�
	while (true)
	{
		//���Hook�Ƿ񱻻�ԭ
		bac->CheckHookPointer();
		//ɨ������ڴ�CRC32
		bac->CheckMemoryCRC32();


		::Sleep(120000);
	}

	baclog->FunctionLog(__FUNCTION__, "Leave");
#if NDEBUG
	VMProtectEnd();
#endif
}

