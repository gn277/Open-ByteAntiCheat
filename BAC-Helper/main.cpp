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

	//实例化BACHelper
	bac_helper = new BACHelper();

	//这里可以读一些配置信息

	//这里做BAC-Base模块的版本检测，如未更新将进行更新操作

	//向主进程注入BAC-Base.dll
#if _WIN64
	if (!bac_helper->ImportTableInject("ByteAntiCheat\\TestGame.exe", "TestGame.exe", "ByteAntiCheat\\BAC-Base64.dll", "..BACBaseInitialize"))
#elif _WIN32
	if (!bac_helper->ImportTableInject("ByteAntiCheat\\TestGame.exe", "TestGame.exe", "ByteAntiCheat\\BAC-Base.dll", "..BACBaseInitialize"))
#endif
	{
		MessageBoxA(NULL, "An error occurred while starting the game", "Error", MB_OK);
		exit(-1);
	}

	//使用BAC-Helper启动注入BAC-Base模块后的新游戏进程



	//释放BACHelper
	if (bac_helper)
		delete bac_helper;

	return 0;
}

