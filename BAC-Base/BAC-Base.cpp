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
#if NDEBUG
	VMProtectBegin("BACBaseInitialize");
#endif

	//ʵ����BAC��־����
	baclog = new BACLog();
	//ʵ����BAC����
	bac = new BAC();

#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Enter");
#endif

	//���ö����쳣���˺���
	::SetUnhandledExceptionFilter(&UnHandleException);

	//�������� ������δǩ���˴��ᴥ���쳣���£���������ǩ��
	if (!bac->InitializeBACKernel())
		throw "initialize bac kernel error!";

	//Hookǰ����CRC32
	printf("old hash:%d\n", bac->CRC32(::GetModuleHandleA("ntdll.dll"), 0x10000));

	//Ӧ�ò�����hook
	bac->HideHook();
	//����LdrLoadDllע��
	bac->MonitorLdrLoadDll();
	//�������뷨ע��
	bac->MonitorImm();
	//�����ڴ��д����
	bac->MonitorMemoryOption();
	//����ѭ���¼�
	bac->LoopEvent();


	//���Ӵ��ڴ�������غ���
	bac->MonitorCreateWindow();

	//Hook�����CRC32
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

	//�ͷ�BAC����
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

	//�ͷ�BAC��־����
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

