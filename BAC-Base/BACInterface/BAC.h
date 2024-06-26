#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include <utility>

#include "BACError.h"
#include "BACClient/BACClient.h"
#include "BACKernelApi/BACKernel.h"
#include "../BACLog/BACLog.h"
#include "../BACTools/BACTools.h"

#include <detours.h>
#include <VMProtectSDK/VMProtectSDK.h>


class BACError :public std::exception
{
private:
	std::string _error_str;

public:
	BACError(const char* error);
	BACError(std::string error);
	~BACError();

public:
	virtual char const* what() const throw();

};


class BAC :public BACKernel, public BACTools
{
private:
	//BAC-Base模块句柄
	HMODULE _self_module = NULL;

	//BAC线程列表<线程函数名,线程句柄>
	std::map<std::string, HANDLE> bac_thread_list;

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
	//监视LdrLoadDll注入
	void MonitorInjecterOperation();
	//监视线程操作
	void MonitorThreadOperation();
	//监视APC注入
	void MonitorApc();
	//监视输入法注入
	void MonitorImm();
	//监视内存读写操作
	void MonitorMemoryOperation();
	//监视窗口创建相关函数
	void MonitorWindowOperation();

	//重新映射内存
	bool RemapImage(ULONG_PTR image_base);
	
	//处理循环事件
	static void LoopEvent();
	void InitializeLoopEnvent();

	//清除匿名进程句柄
	static void ClearOtherProcessHandle();

	//处理内核消息的线程
	static void RecvKernelMessage();
	//驱动消息派遣函数
	void KernelMessageDispathcer(void* message);

	//初始化BACClient
	BASE_ERROR InitializeBACClient();

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
	//添加BAC线程句柄
	void AppendBACThreadHandle(std::string function_name, HANDLE thread_handle);
	//清除BAC线程句柄
	void EraseBACThreadHandle(const std::string function_name);

public:
	HMODULE GetSelfModuleHandle();

public:
	BAC(HMODULE self_module_handle);
	~BAC();

};


extern "C" std::shared_ptr<BAC> bac;
extern "C" std::shared_ptr<BACClient> client;
extern "C" std::shared_ptr<BACLog> baclog;
