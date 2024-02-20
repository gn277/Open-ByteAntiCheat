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


class BAC : public BACKernel, public Tools
{
private:
	//Hook点容器，记录成功Hook地址后续循环效验是否被恢复
#if _WIN64
	std::map<const std::string/*Api名*/, std::map<DWORD64/*地址*/, unsigned int/*Hook后CRC32值*/>> _hook_list;
#else
	std::map<const std::string/*Api名*/, std::map<DWORD/*地址*/, unsigned int/*Hook后CRC32值*/>> _hook_list;
#endif

	//CRC32List结构：std::map<模块路径,<内存起始地址,内存大小>>
	std::map<std::string, std::map<DWORD64, SIZE_T>> _crc32_list;

	//内存白名单结构：std::map<模块名,<内存大小,<内存起始地址,内存结束地址>>>
	std::map<std::string, std::map<SIZE_T, std::map<DWORD64, DWORD64>>> _memory_whitelist;

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


	//重新映射内存
	bool RemapImage(ULONG_PTR image_base);
	
	//处理循环事件
	static void LoopEvent();
	void InitializeLoopEnvent();

	//处理内核消息的线程
	static void RecvKernelMessage();
	//驱动消息派遣函数
	void KernelMessageDispathcer(void* message);

private:
	//获取内存CRC32列表
	bool InitMemoryCRC32List();

public:
	//对内存数据做CRC32效验
	unsigned int CRC32(void* pdata, size_t data_len);
	//检查Hook点状态
	void CheckHookPointer();
	//检查进程模块内存
	void CheckMemoryCRC32();

public:
	BAC();
	~BAC();

};