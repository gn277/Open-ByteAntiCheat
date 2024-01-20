#include "BACKernel.h"
#include "../../BAC-Base.h"


BACKernel::BACKernel()
{
	//获取模块路径
	TCHAR module_path[MAX_PATH] = { 0 };
	GetModuleFileNameW(self_module, module_path, _countof(module_path));
	PathRemoveFileSpecW(module_path);
	_stprintf_s(this->_driver_full_path, _countof(this->_driver_full_path), _T("%s\\%s"), module_path, DRIVER_FILE_NAME);

}

BACKernel::~BACKernel()
{
}

bool BACKernel::InstiallDriver()
{
//#if NDEBUG
//	VMProtectBegin("BACKernel::InstallDriver");
//#endif
#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Enter");
#endif

	//加载前先尝试卸载驱动
	this->UnInstallDriver();

	SC_HANDLE h_service_mgr = NULL;
	SC_HANDLE h_service_ddk = NULL;
	BOOL b_ret = FALSE;

	h_service_mgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == h_service_mgr)
	{
		if (NULL != h_service_mgr)
			CloseServiceHandle(h_service_mgr);
		if (NULL != h_service_ddk)
			CloseServiceHandle(h_service_ddk);
#if _DEBUG
		baclog->FileLog("OpenSCManager error");
		baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
		return FALSE;
	}

	TCHAR driver_name[MAX_PATH] = { 0 };
	_tcscpy_s(driver_name, _countof(driver_name), this->_driver_full_path);
	PathStripPath(driver_name);

	h_service_ddk = CreateService(h_service_mgr,
		driver_name,					// 驱动程序的在注册表中的名字    
		driver_name,					// 注册表驱动程序的 DisplayName 值    
		SERVICE_ALL_ACCESS,				// 加载驱动程序的访问权限    
		SERVICE_KERNEL_DRIVER,			// 表示加载的服务是驱动程序    
		SERVICE_DEMAND_START,			// 注册表驱动程序的 Start 值    
		SERVICE_ERROR_IGNORE,			// 注册表驱动程序的 ErrorControl 值    
		this->_driver_full_path,		// 注册表驱动程序的 ImagePath 值    
		NULL,							// GroupOrder HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\GroupOrderList
		NULL,
		NULL,
		NULL,
		NULL);

	//打开服务
	h_service_ddk = OpenService(h_service_mgr, driver_name, SERVICE_ALL_ACCESS);

	//开启服务
	b_ret = StartService(h_service_ddk, NULL, NULL);

	if (NULL != h_service_mgr)
		CloseServiceHandle(h_service_mgr);
	if (NULL != h_service_ddk)
		CloseServiceHandle(h_service_ddk);

#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
	return b_ret;
//#if NDEBUG
//	VMProtectEnd();
//#endif
}

bool BACKernel::UnInstallDriver()
{
#if NDEBUG
	VMProtectBegin("BACKernel::UnInstallDriver");
#endif
#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Enter");
#endif

	//正式执行卸载
	SC_HANDLE h_service_mgr = NULL;
	SC_HANDLE h_service_ddk = NULL;
	SERVICE_STATUS svr_sta;
	BOOL bRet = TRUE;
	//打开SCM管理器  
	h_service_mgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (h_service_mgr == NULL)
	{
#if _DEBUG
		baclog->FileLog("OpenSCManager error");
		baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
		if (NULL != h_service_mgr)
			CloseServiceHandle(h_service_mgr);
		if (NULL != h_service_ddk)
			CloseServiceHandle(h_service_ddk);
		return FALSE;
	}

	//打开驱动所对应的服务  
	TCHAR driver_name[MAX_PATH] = { 0 };
	_tcscpy_s(driver_name, _countof(driver_name), this->_driver_full_path);
	PathStripPath(driver_name);

	h_service_ddk = OpenService(h_service_mgr, driver_name, SERVICE_ALL_ACCESS);
	if (NULL == h_service_ddk)
	{
#if _DEBUG
		baclog->FileLog("OpenService error");
		baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
		if (NULL != h_service_mgr)
			CloseServiceHandle(h_service_mgr);
		if (NULL != h_service_ddk)
			CloseServiceHandle(h_service_ddk);
		return FALSE;
	}

	if (!ControlService(h_service_ddk, SERVICE_CONTROL_STOP, &svr_sta))
	{
#if _DEBUG
		baclog->FileLog("ControlService error");
		baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
		if (NULL != h_service_mgr)
			CloseServiceHandle(h_service_mgr);
		if (NULL != h_service_ddk)
			CloseServiceHandle(h_service_ddk);
		return FALSE;
	}

	//动态卸载驱动程序
	DeleteService(h_service_ddk);

#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
	if (NULL != h_service_mgr)
		CloseServiceHandle(h_service_mgr);
	if (NULL != h_service_ddk)
		CloseServiceHandle(h_service_ddk);
	return TRUE;

#if NDEBUG
	VMProtectEnd();
#endif
}

bool BACKernel::OpenDriverHandle()
{
//#if NDEBUG
//	VMProtectBegin("BACKernel::OpenDriverHandle");
//#endif
#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Enter");
#endif

	//read_able_event = CreateEvent(NULL, FALSE, FALSE, NULL);
	this->_driver_handle = CreateFileW(
		DRIVER_LINKER_NAME,
		GENERIC_ALL,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (NULL == this->_driver_handle || INVALID_HANDLE_VALUE == this->_driver_handle)
	{
#if _DEBUG
		baclog->FileLog("driver handle open fialed");
		baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
		return FALSE;
	}

#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
	return TRUE;
//#if NDEBUG
//	VMProtectEnd();
//#endif
}



