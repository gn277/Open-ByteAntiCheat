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

	////获取模块路径
	//TCHAR driver_full_path[MAX_PATH] = { NULL };
	//TCHAR module_path[MAX_PATH] = { 0 };
	//GetModuleFileNameW(::GetModuleHandleA(NULL), module_path, _countof(module_path));
	//PathRemoveFileSpecW(module_path);
	//_stprintf_s(driver_full_path, _countof(driver_full_path), _T("%s\\%s"), module_path, _T("ByteAntiCheat\\BAC-Base.sys"));
	////printf("driver full path:%S\n", driver_full_path);
	//
	////获取驱动文件名称
	//TCHAR driver_name[MAX_PATH] = { NULL };
	//_tcscpy_s(driver_name, _countof(driver_name), driver_full_path);
	//PathStripPath(driver_name);
	////printf("driver name:%S\n", driver_name);

	//实例化BACHelper
	bac_helper = new BACHelper();
	//这里可以读一些配置信息
	bac_config = new BACConfig();

	//读取配置文件中的游戏进程名 自己在ByteAntiCheat目录下创建BACConfig.ini文件其中写GameFullName=TestGame.exe，发布时带着配置文件一起解压
	std::string gamefile_full_name;
	bac_config->ReadConfig("GameFullName", &gamefile_full_name);
	//std::cout << "gamefile_full_name:" << gamefile_full_name.c_str() << endl;

	//这里做BAC-Base模块的版本检测，如未更新将进行更新操作

	//向主进程注入BAC-Base.dll
#if _WIN64
	if (!bac_helper->ImportTableInject(std::string("ByteAntiCheat\\" + gamefile_full_name).c_str(), gamefile_full_name.c_str(), "ByteAntiCheat\\BAC-Base64.dll", "..BACBaseInitialize"))
#elif _WIN32
	if (!bac_helper->ImportTableInject(std::string("ByteAntiCheat\\" + gamefile_full_name).c_str(), gamefile_full_name.c_str(), "ByteAntiCheat\\BAC-Base.dll", "..BACBaseInitialize"))
#endif
	{
		MessageBoxA(NULL, "An error occurred while starting the game", "Error", MB_OK);
		exit(-1);
	}

	//使用BAC-Helper启动注入BAC-Base模块后的新游戏进程



	//释放BACConfig
	if (bac_config)
		delete bac_config;
	//释放BACHelper
	if (bac_helper)
		delete bac_helper;

	std::cout << "Launcher over ..." << std::endl;

	//HACCEL acc_table = LoadAccelerators(hInstance, MAKEINTRESOURCE(NULL));
	//
	////主消息循环:
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

	//处理退出事件
	OutputDebugStringA("[BAC]:BAC-Helper exit()\n");

	//return (int)msg.wParam;
	return 0;
}

