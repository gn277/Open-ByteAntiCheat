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

using namespace std;


class BAC
{
private:

private:
	void MakePePacked(HANDLE hProcess, PBYTE pImageBuff);

public:
	//隐藏应用层的hook
	void HideHook();
	//监视窗口创建相关函数
	void MonitorCreateWindow();
	//监视LdrLoadDll注入
	void MonitorLdrLoadDll();
	//监视APC注入
	void MonitorApc();
	//监视输入法注入
	void MonitorImm();
	//监视内存读写操作
	void MonitorMemoryOption();
	
	//处理循环事件
	void LoopEvent();

public:
	//对内存数据做CRC32效验
	unsigned int CRC32(void* pdata, size_t data_len);

public:
	BAC();
	~BAC();

};