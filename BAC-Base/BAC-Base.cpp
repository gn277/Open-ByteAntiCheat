////////////////////////////////////////////////////////////////
//						ʵ�ֶ�����õĽӿ�
////////////////////////////////////////////////////////////////
#include "BAC-Base.h"
#include "BACInterface/BAC.h"


LONG CALLBACK UnHandleException(EXCEPTION_POINTERS* p_exception)
{
	bac->BACKernel::UnInstallDriver();

	return EXCEPTION_CONTINUE_SEARCH;
}

bool BACBaseInitialize(HMODULE module_handle)
{
#if _DEBUG
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
#endif

	//ʵ����BAC��־
	baclog = std::make_shared<BACLog>();
	try
	{
		//ʵ����BAC
		bac = std::make_shared<BAC>(module_handle);

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
		GetModuleFileNameW(module_handle, module_path, _countof(module_path));
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
		//�����̲߳���
		bac->MonitorThreadOperation();
		//����DLLע��
		bac->MonitorInjecterOperation();
		//�������뷨ע��
		bac->MonitorImm();
		//�����ڴ��д����
		bac->MonitorMemoryOperation();
		//���Ӵ��ڴ�������غ���
		bac->MonitorWindowOperation();

		////�ڴ������ɺ�ӳ���ڴ�ﵽ���벻���޸�
		//if (!bac->RemapImage((ULONG_PTR)self_module))
		//	MessageBoxA(NULL, "BAC Initialize error!", "ERROR", MB_OK);
		//
		//MODULEINFO module_info = { NULL };
		//::GetModuleInformation(::GetCurrentProcess(), self_module, &module_info, sizeof(module_info));
		//if (!bac->BACKernel::RemapImage("BAC-Base64.dll", (HANDLE)::GetCurrentProcessId(), (DWORD64)self_module, module_info.SizeOfImage))
		//	MessageBoxA(NULL, "BAC Initialize error!", "ERROR", MB_OK);

#if NDEBUG
	//�ں˱�������
		if (!bac->BACKernel::ProtectProcessByName(process_name))
		{
			if (MessageBoxA(::GetActiveWindow(), "driver load error,please check!", "BAC:Error", MB_OK))
				ExitProcess(-1);
			else
				ExitProcess(-2);
		}
		//����Ӧ�ò��ļ���д�¼����
		if (!bac->BACKernel::SendFileEventToKernel())
		{
			MessageBoxA(::GetActiveWindow(), "send file event to kernel error", "BAC::Error", MB_OK);
			ExitProcess(-1);
		}
		//�����Ϸ���̵ĵ��Զ˿�
		if (!bac->BACKernel::ClearProcessDebugPort((HANDLE)::GetCurrentProcessId()))
		{
			MessageBoxA(::GetActiveWindow(), "clear process debug port error", "BAC::Error", MB_OK);
			ExitProcess(-1);
		}

		//����������̾��
		bac->AppendBACThreadHandle(
			"BAC::ClearOtherProcessHandle",
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)BAC::ClearOtherProcessHandle, NULL, NULL, NULL));
#endif

		//����ѭ���¼�
		bac->AppendBACThreadHandle(
			"BAC::LoopEvent",
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)BAC::LoopEvent, NULL, NULL, NULL));

		//���ӷ�����
		client = std::make_shared<BACClient>();

		baclog->FileLogf("%s-> %s: %s", "[BAC]", __FUNCTION__, "Leave");
		return true;
	}
	catch (const std::shared_ptr<BACError>& e)
	{
		baclog->FileLog(e->what());
		return false;
	}
	catch (const std::shared_ptr<BACClientError>& e)
	{
		baclog->FileLog(e->what());
		return false;
	}
	catch (const std::exception& e)
	{
		baclog->FileLog(std::string(__FUNCTION__) + "had exception: " + e.what());
		return false;
	}

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

	//���BACʵ�����ɹ���ж������
	if (bac)
	{
		if(!bac->BACKernel::UnInstallDriver())
			throw "uninitialize bac kernel error";
	}

	baclog->FunctionLog(__FUNCTION__, "Leave");

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
			if (!BACBaseInitialize(h_module))
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

