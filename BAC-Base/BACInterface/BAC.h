#pragma once
#include <Windows.h>
#include <stdio.h>
#include <iostream>

#include "../Detours/include/detours.h"
#if _WIN64
#pragma comment(lib,"Detours/lib.X64/detours.lib")
#elif _WIN32
#pragma comment(lib,"Detours/lib.X86/detours.lib")
#endif

#include "SystemApi.h"


class BAC
{
private:

	//隐藏应用层的hook
public:
	void HideHook();
private:
	void MakePePacked(HANDLE hProcess, PBYTE pImageBuff);

	//监视窗口创建相关函数
public:
	void MonitorCreateWindow();
private:


	//Hook功能的回调函数
public:
	//static 

public:
	BAC();
	~BAC();

};