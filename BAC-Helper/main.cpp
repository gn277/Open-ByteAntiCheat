#include <Windows.h>
#include <stdio.h>
#include <iostream>

#include "BAC-Helper.h"

BACHelper* bac_helper = nullptr;


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpcmdLine, _In_ int nCmdShow)
{
#if _DEBUG
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
#endif

	//ʵ����BACHelper
	bac_helper = new BACHelper();

	//������Զ�һЩ������Ϣ

	//������BAC-Baseģ��İ汾��⣬��δ���½����и��²���

	//��������ע��BAC-Base.dll
#if _WIN64
	if (!bac_helper->ImportTableInject("ByteAntiCheat\\TestGame.exe", "TestGame.exe", "ByteAntiCheat\\BAC-Base64.dll", "..BACBaseInitialize"))
#elif _WIN32
	if (!bac_helper->ImportTableInject("ByteAntiCheat\\TestGame.exe", "TestGame.exe", "ByteAntiCheat\\BAC-Base.dll", "..BACBaseInitialize"))
#endif
	{
		MessageBoxA(NULL, "An error occurred while starting the game", "Error", MB_OK);
		exit(-1);
	}

	//ʹ��BAC-Helper����ע��BAC-Baseģ��������Ϸ����



	//�ͷ�BACHelper
	if (bac_helper)
		delete bac_helper;

	return 0;
}

