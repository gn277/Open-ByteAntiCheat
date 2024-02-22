////////////////////////////////////////////////////////////////
//						ʵ�ֶ�����õĽӿ�
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

	TCHAR driver_name[MAX_PATH] = { NULL };
	TCHAR module_path[MAX_PATH] = { NULL };
	TCHAR process_path[MAX_PATH] = { NULL };
	TCHAR process_name[MAX_PATH] = { NULL };
	TCHAR driver_full_path[MAX_PATH] = { NULL };

	//��ȡBAC-Baseģ������·��
	GetModuleFileNameW(self_module, module_path, _countof(module_path));
	//��ȡ����·��
	PathRemoveFileSpecW(module_path);
	_stprintf_s(driver_full_path, _countof(driver_full_path), _T("%s\\%s"), module_path, DRIVER_FILE_NAME);
	//��ȡ������
	GetModuleFileNameW(::GetModuleHandleW(NULL), process_path, _countof(process_path));
	wcscpy(process_name, wcsrchr(process_path, '\\') + 1);
	//��ȡ�����ļ�����
	wcscpy_s(driver_name, _countof(driver_name), driver_full_path);
	PathStripPathW(driver_name);

	//�������� ������δǩ���˴��ᴥ���쳣���£���������ǩ��
	if (!bac->BACKernel::InstiallDriver(driver_name, driver_full_path))
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
	//


	////�ڴ������ɺ�ӳ���ڴ�ﵽ���벻���޸�
	//if (!bac->RemapImage((ULONG_PTR)self_module))
	//	MessageBoxA(NULL, "BAC Initialize error!", "ERROR", MB_OK);
	//
	//MODULEINFO module_info = { NULL };
	//::GetModuleInformation(::GetCurrentProcess(), self_module, &module_info, sizeof(module_info));
	//if (!bac->BACKernel::RemapImage("BAC-Base64.dll", (HANDLE)::GetCurrentProcessId(), (DWORD64)self_module, module_info.SizeOfImage))
	//	MessageBoxA(NULL, "BAC Initialize error!", "ERROR", MB_OK);

#if NDEBUG
	//�����Ϸ���̵ĵ��Զ˿�
	if (!bac->BACKernel::ClearProcessDebugPort((HANDLE)::GetCurrentProcessId()))
	{
		MessageBoxA(::GetActiveWindow(), "clear process debug port error", "BAC::Error", MB_OK);
		ExitProcess(-1);
	}
	//�ں˱�������
	if (!bac->BACKernel::ProtectProcessByName(process_name))
	{
		if (MessageBoxA(::GetActiveWindow(), "driver load error,please check!", "BAC:Error", MB_OK))
			ExitProcess(-1);
		else
			ExitProcess(-2);
	}
#endif

	//����ѭ���¼�
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

	//�ͷ�BAC����
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

