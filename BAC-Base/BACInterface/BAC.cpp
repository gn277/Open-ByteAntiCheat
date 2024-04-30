#include "BAC.h"
#include "../BAC-Base.h"


std::shared_ptr<BAC> bac = nullptr;
std::shared_ptr<BACClient> client = nullptr;
std::shared_ptr<BACLog> baclog = nullptr;


BACError::BACError(const char* error) :_error_str(error)
{
}

BACError::BACError(std::string error) :_error_str(error)
{
}

BACError::~BACError()
{
}

char const* BACError::what() const throw()
{
	return this->_error_str.c_str();
}


BAC::BAC(HMODULE self_module_handle) :_self_module(self_module_handle)
{
}

BAC::~BAC()
{
}

void BAC::AppendBACThreadHandle(std::string function_name, HANDLE thread_handle)
{
	this->bac_thread_list.insert(std::make_pair(function_name, thread_handle));
}

HMODULE BAC::GetSelfModuleHandle()
{
	return this->_self_module;
}

BASE_ERROR BAC::InitializeBACClient()
{
#if NDEBUG
	VMProtectBeginUltra("BAC::InitializeBACClient");
#endif
	baclog->FunctionLog(__FUNCTION__, "Enter");

	//连接TCP服务器
	auto status = client->ConnectTcpServer("127.0.0.1", 999);
	if (status != BASE_SUCCESS)
		return status;

	//发送登录命令
	status = client->Login("BacUser_1");
	if (status != BASE_SUCCESS)
		return status;

	//发送机器信息
	status = client->SendMachineInfo();
	if (status != BASE_SUCCESS)
		return status;

	baclog->FunctionLog(__FUNCTION__, "Leave");
#if NDEBUG
	VMProtectEnd();
#endif
	return BASE_SUCCESS;
}

