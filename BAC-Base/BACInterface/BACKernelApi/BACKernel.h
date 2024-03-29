#pragma once
#include <Windows.h>
#include <tchar.h>
#include <shlwapi.h>

#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib, "user32.lib")

#include "../../../BAC-Base-Kernel/IRPControl/BAC-DataStruct.h"
#include "../../../BAC-Base-Kernel/IRPControl/IRPControlCode.h"

//驱动文件名称
#define DRIVER_FILE_NAME _T("BAC-Base.sys")


class BACKernel
{
private:
	//驱动文件句柄
	HANDLE _driver_handle = NULL;
	//应用层文件读写句柄(用于内核的文件读写通讯)
	HANDLE _file_event_handle = NULL;

	//驱动文件路径
	TCHAR _driver_full_path[MAX_PATH] = { NULL };
	TCHAR _driver_name[MAX_PATH] = { NULL };

private:
	//发送数据包到内核
	bool SendPacketToKernel(int message_number, void* buffer, int buffer_len);

public:
	BACKernel();
	~BACKernel();

private:
	bool ServiceExists(const wchar_t* service_name);
	void DriverEventLogUninstall(const wchar_t* service_name);

public:
	bool InstiallDriver(const wchar_t* driver_name, const wchar_t* driver_path);
	bool UnInstallDriver();
	bool OpenDriverHandle();

public:
	//发送应用层文件读写句柄到内核
	bool SendFileEventToKernel();
	//内核回调保护进程
	bool ProtectProcessByName(const wchar_t* process_name);
	//清空进程调试端口
	bool ClearProcessDebugPort(IN HANDLE pid);
	bool RemapImage(const char* module_name, HANDLE pid, DWORD64 memory_address, DWORD memory_size);

};

