#include <Windows.h>
#include <stdio.h>
#include <iostream>

#include "BAC-Helper.h"
#include "BAC-Config/BAC-Config.h"

BACHelper* bac_helper = nullptr;
BACConfig* bac_config = nullptr;


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpcmdLine, _In_ int nCmdShow)
{
#if _DEBUG
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
#endif

	//ʵ����BACHelper
	bac_helper = new BACHelper();
	//������Զ�һЩ������Ϣ
	bac_config = new BACConfig();

	//��ȡ�����ļ��е���Ϸ������
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

	HACCEL acc_table = LoadAccelerators(hInstance, MAKEINTRESOURCE(NULL));

	//����Ϣѭ��:
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, acc_table, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	//�����˳��¼�


	return (int)msg.wParam;
}

