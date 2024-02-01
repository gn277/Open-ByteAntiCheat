#pragma once
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include <utility>

#include "../Detours/include/detours.h"
#if _WIN64
#pragma comment(lib,"Detours/lib.X64/detours.lib")
#elif _WIN32
#pragma comment(lib,"Detours/lib.X86/detours.lib")
#endif

#include "SystemApi.h"
#include "BACKernelApi/BACKernel.h"
#include "../BACTools/BACTools.h"

using namespace std;


class BAC : public Tools
{
private:
	BACKernel* bac_kernel = nullptr;

	//Hook点容器，记录成功Hook地址后续循环效验是否被恢复
#if _WIN64
	std::map<const std::string/*Api名*/, std::map<DWORD64/*地址*/, unsigned int/*Hook后CRC32值*/>> _hook_list;
#else
	std::map<const std::string/*Api名*/, std::map<DWORD/*地址*/, unsigned int/*Hook后CRC32值*/>> _hook_list;
#endif

private:
	void MakePePacked(HANDLE hProcess, PBYTE pImageBuff);
	bool JudgmentHookModule(PVOID hook_address);

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
	static void LoopEvent();

public:
	//对内存数据做CRC32效验
	unsigned int CRC32(void* pdata, size_t data_len);
	//检查Hook点状态
	void CheckHookPointer();

public:
	//BAC内核接口初始化
	bool InitializeBACKernel();
	bool UnInitializeBACKernel();

public:
	BAC();
	~BAC();

};