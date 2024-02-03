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
	//驱动文件路径
	TCHAR _driver_full_path[MAX_PATH] = { NULL };
	TCHAR _driver_name[MAX_PATH] = { NULL };

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
	bool ProtectProcessByName(const wchar_t* process_name);

};

