////////////////////////////////////////////////////////////////
//						�ڴ˴���ѭ���¼�
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

	//��һЩ��ʼ��������ٴ����¼�
	bac->InitializeLoopEnvent();

	//����ѭ���¼�
	while (true)
	{
		//���Hook�Ƿ񱻻�ԭ
		bac->CheckHookPointer();
		//ɨ������ڴ�CRC32
		bac->CheckMemoryCRC32();
#if NDEBUG
		//�����Ϸ���̵ĵ��Զ˿�
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

