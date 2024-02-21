#include "BACKernel.h"
#include "../../BAC-Base.h"


BACKernel::BACKernel()
{
}

BACKernel::~BACKernel()
{
}

bool BACKernel::ServiceExists(const wchar_t* service_name)
{
	SC_HANDLE control_handle;
	SC_HANDLE service_handle;

	control_handle = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);

	if (control_handle == NULL)
	{
#if _DEBUG
		baclog->FileLog(std::string("ServiceExists: Failed to open Service Control Manager. errorcode:" + GetLastError()).c_str());
#endif
		return FALSE;
	}

	service_handle = OpenService(control_handle, service_name, SERVICE_START | SERVICE_STOP | SERVICE_QUERY_STATUS);

	if (service_handle == NULL)
	{
#if _DEBUG
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
		baclog->FileLog(std::string("ServiceExists: Failed to open Service:" + convert.to_bytes(service_name)).c_str());
#endif
		CloseServiceHandle(control_handle);
		return FALSE;
	}

	CloseServiceHandle(service_handle);
	CloseServiceHandle(control_handle);

	return true;
}

void BACKernel::DriverEventLogUninstall(const wchar_t* service_name)
{
#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Enter");
#endif

	HKEY key;

	if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"System\\CurrentControlSet\\Services\\EventLog\\System", 0, KEY_ALL_ACCESS, &key) != ERROR_SUCCESS)
		return;
	RegDeleteKeyW(key, service_name);

	RegCloseKey(key);

#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
}

bool BACKernel::InstiallDriver(const wchar_t* driver_name, const wchar_t* driver_path)
{
	baclog->FunctionLog(__FUNCTION__, "Enter");

	wcscpy(this->_driver_name, driver_name);
	wcscpy(this->_driver_full_path, driver_path);

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

		baclog->FileLog("OpenSCManager error");
		baclog->FunctionLog(__FUNCTION__, "Leave");

		return FALSE;
	}

	h_service_ddk = CreateServiceW(h_service_mgr,
		this->_driver_name,						// 驱动程序的在注册表中的名字    
		this->_driver_name,						// 注册表驱动程序的 DisplayName 值    
		SERVICE_ALL_ACCESS,						// 加载驱动程序的访问权限    
		SERVICE_KERNEL_DRIVER,					// 表示加载的服务是驱动程序    
		SERVICE_DEMAND_START,					// 注册表驱动程序的 Start 值    
		SERVICE_ERROR_IGNORE,					// 注册表驱动程序的 ErrorControl 值    
		this->_driver_full_path,				// 注册表驱动程序的 ImagePath 值    
		NULL,									// GroupOrder HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\GroupOrderList
		NULL,
		NULL,
		NULL,
		NULL);

	//打开服务
	h_service_ddk = OpenServiceW(h_service_mgr, this->_driver_name, SERVICE_ALL_ACCESS);

	//开启服务
	b_ret = StartServiceW(h_service_ddk, NULL, NULL);

	if (NULL != h_service_mgr)
		CloseServiceHandle(h_service_mgr);
	if (NULL != h_service_ddk)
		CloseServiceHandle(h_service_ddk);

	this->DriverEventLogUninstall(this->_driver_name);

	baclog->FunctionLog(__FUNCTION__, "Leave");

	//完成后打开驱动句柄
	return this->OpenDriverHandle();
}

bool BACKernel::UnInstallDriver()
{
#if NDEBUG
	VMProtectBeginUltra("BACKernel::UnInstallDriver");
#endif
	baclog->FunctionLog(__FUNCTION__, "Enter");

	//卸载前判断服务是否存在
	if (!this->ServiceExists(this->_driver_name))
	{
		baclog->FunctionLog(__FUNCTION__, "Leave");
		return true;
	}
	this->DriverEventLogUninstall(this->_driver_name);

	//正式执行卸载
	SC_HANDLE h_service_mgr = NULL;
	SC_HANDLE h_service_ddk = NULL;
	SERVICE_STATUS svr_sta;
	BOOL bRet = TRUE;
	//打开SCM管理器  
	h_service_mgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (h_service_mgr == NULL)
	{
		baclog->FileLog("OpenSCManager error");
		baclog->FunctionLog(__FUNCTION__, "Leave");

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
		baclog->FileLog("OpenService error");
		baclog->FunctionLog(__FUNCTION__, "Leave");

		if (NULL != h_service_mgr)
			CloseServiceHandle(h_service_mgr);
		if (NULL != h_service_ddk)
			CloseServiceHandle(h_service_ddk);
		return FALSE;
	}

	if (!ControlService(h_service_ddk, SERVICE_CONTROL_STOP, &svr_sta))
	{
		baclog->FileLog("ControlService error");
		baclog->FunctionLog(__FUNCTION__, "Leave");

		if (NULL != h_service_mgr)
			CloseServiceHandle(h_service_mgr);
		if (NULL != h_service_ddk)
			CloseServiceHandle(h_service_ddk);
		return FALSE;
	}

	//动态卸载驱动程序
	DeleteService(h_service_ddk);

	if (NULL != h_service_mgr)
		CloseServiceHandle(h_service_mgr);
	if (NULL != h_service_ddk)
		CloseServiceHandle(h_service_ddk);

	baclog->FunctionLog(__FUNCTION__, "Leave");
	return TRUE;

#if NDEBUG
	VMProtectEnd();
#endif
}

typedef struct _send_event_handle
{
	int event_handle;
}send_read_able_event_to_driver;

bool BACKernel::OpenDriverHandle()
{
	baclog->FunctionLog(__FUNCTION__, "Enter");

	//应用层文件读写句柄
	this->_file_handle = CreateEventW(NULL, FALSE, FALSE, NULL);
	//驱动文件句柄
	this->_driver_handle = CreateFileW(DRIVER_LINKER_NAME, GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if (NULL == this->_driver_handle || INVALID_HANDLE_VALUE == this->_driver_handle)
	{
		baclog->FileLog("driver handle open fialed");
		baclog->FunctionLog(__FUNCTION__, "Leave");

		return FALSE;
	}

	////将应用层文件读写句柄发到内核层
	//printf("file_handle 句柄：%p\n", this->_file_handle);
	//send_read_able_event_to_driver st = { (int)this->_file_handle };
	//this->SendPacketToKernel(SEND_FILE_EVENT_HANDLE, &st, sizeof(send_read_able_event_to_driver));
	////this->SendPacketToKernel(SEND_FILE_EVENT_HANDLE, &this->_file_handle, sizeof(this->_file_handle));
	
	char test[13] = "test message";
	this->SendPacketToKernel(SEND_FILE_EVENT_HANDLE, &test, sizeof(test));

	baclog->FunctionLog(__FUNCTION__, "Leave");
	return TRUE;
}

bool BACKernel::SendPacketToKernel(int message_number, void* buffer, int buffer_len)
{
	int new_packet_len = (sizeof(int) * 2) + buffer_len;
	PPacketStruct p_packet = (PPacketStruct)new char[new_packet_len];
	memset(p_packet, 0, new_packet_len);

	//将消息号加入数据包中
	p_packet->packet_number = message_number;
	//将数据长度加入数据包中
	p_packet->buffer_len = buffer_len;
	//将消息写入消息号后
	memcpy(&p_packet->buffer, buffer, buffer_len);

	char data[] = "Test data!!!";
	DWORD real_write = NULL;
	bool ret = WriteFile(this->_driver_handle, (PVOID)data, strlen(data), &real_write, NULL);
	//bool ret = WriteFile(this->_driver_handle, (PVOID)p_packet, new_packet_len, &real_write, NULL);

	delete[] p_packet;
	//return ret;
	return true;
}

bool BACKernel::ProtectProcessByName(const wchar_t* process_name)
{
	ULONG dw_write;
	PVOID return_buffer = NULL;
	ProtectProcessStruct data = { NULL };

	wcscpy(data.file_path, process_name);

	DeviceIoControl(this->_driver_handle, ProtectProcess_Code, (PVOID)&data, sizeof(data), &return_buffer, sizeof(return_buffer), &dw_write, NULL);

	if (NT_SUCCESS(return_buffer)) return true;
	else return false;
}

bool BACKernel::RemapImage(const char* module_name, HANDLE pid, DWORD64 memory_address, DWORD memory_size)
{
	ULONG dw_write;
	PVOID return_buffer = NULL;
	ProtectMemoryStruct data = { NULL };

	strcpy(data.module_name, module_name);
	data.process_id = pid;
	data.memory_address = memory_address;
	data.memory_size = memory_size;

	DeviceIoControl(this->_driver_handle, MemoryProtect_Code, (PVOID)&data, sizeof(data), &return_buffer, sizeof(return_buffer), &dw_write, NULL);

	if (NT_SUCCESS(return_buffer)) return true;
	else return false;
}



