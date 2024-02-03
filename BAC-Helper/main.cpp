#include <Windows.h>
#include <stdio.h>
#include <iostream>

#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

#include "BAC-Helper.h"
#include "BAC-Config/BAC-Config.h"

BACHelper* bac_helper = nullptr;
BACConfig* bac_config = nullptr;


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpcmdLine, _In_ int nCmdShow)
{
//#if _DEBUG
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
//#endif
	OutputDebugStringA("[BAC]:BAC-Helper init()\n");

	////��ȡģ��·��
	//TCHAR driver_full_path[MAX_PATH] = { NULL };
	//TCHAR module_path[MAX_PATH] = { 0 };
	//GetModuleFileNameW(::GetModuleHandleA(NULL), module_path, _countof(module_path));
	//PathRemoveFileSpecW(module_path);
	//_stprintf_s(driver_full_path, _countof(driver_full_path), _T("%s\\%s"), module_path, _T("ByteAntiCheat\\BAC-Base.sys"));
	////printf("driver full path:%S\n", driver_full_path);
	//
	////��ȡ�����ļ�����
	//TCHAR driver_name[MAX_PATH] = { NULL };
	//_tcscpy_s(driver_name, _countof(driver_name), driver_full_path);
	//PathStripPath(driver_name);
	////printf("driver name:%S\n", driver_name);

	//ʵ����BACHelper
	bac_helper = new BACHelper();
	//������Զ�һЩ������Ϣ
	bac_config = new BACConfig();

	//��ȡ�����ļ��е���Ϸ������ �Լ���ByteAntiCheatĿ¼�´���BACConfig.ini�ļ�����дGameFullName=TestGame.exe������ʱ���������ļ�һ���ѹ
	std::string gamefile_full_name;
	bac_config->ReadConfig("GameFullName", &gamefile_full_name);
	//std::cout << "gamefile_full_name:" << gamefile_full_name.c_str() << endl;

	//������BAC-Baseģ��İ汾��⣬��δ���½����и��²���

	//��������ע��BAC-Base.dll
#if _WIN64
	if (!bac_helper->ImportTableInject(std::string("ByteAntiCheat\\" + gamefile_full_name).c_str(), gamefile_full_name.c_str(), "ByteAntiCheat\\BAC-Base64.dll", "..BACBaseInitialize"))
#elif _WIN32
	if (!bac_helper->ImportTableInject(std::string("ByteAntiCheat\\" + gamefile_full_name).c_str(), gamefile_full_name.c_str(), "ByteAntiCheat\\BAC-Base.dll", "..BACBaseInitialize"))
#endif
	{
		MessageBoxA(NULL, "An error occurred while starting the game", "Error", MB_OK);
		exit(-1);
	}

	//ʹ��BAC-Helper����ע��BAC-Baseģ��������Ϸ����



	//�ͷ�BACConfig
	if (bac_config)
		delete bac_config;
	//�ͷ�BACHelper
	if (bac_helper)
		delete bac_helper;

	std::cout << "Launcher over ..." << std::endl;

	//HACCEL acc_table = LoadAccelerators(hInstance, MAKEINTRESOURCE(NULL));
	//
	////����Ϣѭ��:
	//MSG msg;
	//while (GetMessage(&msg, nullptr, 0, 0))
	//{
	//	OutputDebugStringA("[BAC]:BAC-Helper running()\n");
	//	if (!TranslateAccelerator(msg.hwnd, acc_table, &msg))
	//	{
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}
	//}

	//�����˳��¼�
	OutputDebugStringA("[BAC]:BAC-Helper exit()\n");

	//return (int)msg.wParam;
	return 0;
}

