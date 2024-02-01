////////////////////////////////////////////////////////////////
//						ʵ�ֶ�����õĽӿ�
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
	//ʵ����BAC��־����
	baclog = new BACLog();
	//ʵ����BAC����
	bac = new BAC();

#if NDEBUG
	VMProtectBeginUltra("BACBaseInitialize");
#endif
	baclog->FunctionLog(__FUNCTION__, "Enter");

	//���ö����쳣���˺���
	::SetUnhandledExceptionFilter(&UnHandleException);

	//�������� ������δǩ���˴��ᴥ���쳣���£���������ǩ��
	if (!bac->InitializeBACKernel())
		throw "initialize bac kernel error!";

	//Ӧ�ò�����hook
	bac->HideHook();
	//����LdrLoadDllע��
	bac->MonitorLdrLoadDll();
	//�������뷨ע��
	bac->MonitorImm();
	//�����ڴ��д����
	bac->MonitorMemoryOption();
	//���Ӵ��ڴ�������غ���
	bac->MonitorCreateWindow();


	//����ѭ���¼�
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

	//�ͷ�BAC����
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

	//�ͷ�BAC��־����
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

