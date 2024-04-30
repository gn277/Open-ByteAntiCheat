#include "BACClient.h"
#include "../BAC.h"


template<typename T1, typename T2>
void OutPutLog(T1 title, T2 data)
{
#if _PRINT_INFORMATION
	std::cout << "[BAC-Base]-> " << title << " : " << data << std::endl;
#else
	return;
#endif
}


BACClientError::BACClientError(const char* error) :_error_str(error)
{
}

BACClientError::BACClientError(std::string error) :_error_str(error)
{
}

BACClientError::~BACClientError()
{
}

char const* BACClientError::what() const throw()
{
	return this->_error_str.c_str();
}


BACClient::BACClient()
{
}

BACClient::~BACClient()
{
}

auto BACClient::ConnectTcpServer(const char* host, int port)->BASE_ERROR
{
	//连接TCP服务器
	auto result = this->TcpServer::BACTcpSocket::Connect(AF_INET, host, port);
	if (result != true)
		return BASE_CONNECT_TCP_SERVER_ERROR;

	return BASE_SUCCESS;
}

auto BACClient::Login(std::string user_id)->BASE_ERROR
{
	baclog->FunctionLog(__FUNCTION__, "Enter");

	//发送登录命令
	if (!this->TcpServer::Send(BACCommand::BAC_CLIENT_LOGIN, user_id))
	{
		baclog->FunctionLog(__FUNCTION__, "Leave");
		return BASE_CLIENT_LOGIN_ERROR;
	}

	//接收服务器响应数据
	std::string response = this->TcpServer::Recv(sizeof(BACPacketHeader));

	//判断收到的命令是否合法
	if (this->TcpServer::GetCommand(response) != BACCommand::BAC_CLIENT_LOGIN)
	{
		baclog->FileLogf("BASE login command error:%d", this->TcpServer::GetCommand(response));
		baclog->FunctionLog(__FUNCTION__, "Leave");
		return BASE_CLIENT_LOGIN_ERROR;
	}

	//处理命令对应状态
	switch (this->TcpServer::GetResponseStatus(response))
	{
		case BACStatus::Success:
		{
			OutPutLog("__FUNCTION__", "client login success");
			baclog->FileLog("BACClient login success");
			break;
		}
		default:
		{
			OutPutLog(__FUNCTION__, "BACStatus error" + std::to_string(this->TcpServer::GetResponseStatus(response)));
			baclog->FunctionLog(__FUNCTION__, "Leave");
			return BASE_CLIENT_LOGIN_ERROR;
		}
	}

	baclog->FunctionLog(__FUNCTION__, "Leave");
	return BASE_SUCCESS;
}

auto BACClient::SendMachineInfo()->BASE_ERROR
{
	SYSTEM_INFO system_info = { NULL };
	::GetSystemInfo(&system_info);

	//发送请求命令
	if (!this->TcpServer::Send(BAC_CLIENT_MACHINE_INFO, "Test machine info..."))
		return BASE_CLIENT_SEND_MACHINE_INFO_ERROR;

	//获取响应信息
	auto response = this->TcpServer::Recv(sizeof(BACPacketHeader));
	//判断命令是否合法
	if (this->TcpServer::GetCommand(response) != BACCommand::BAC_CLIENT_MACHINE_INFO)
		return BASE_CLIENT_SEND_MACHINE_INFO_ERROR;

	//判断状态
	switch (this->TcpServer::GetResponseStatus(response))
	{
		case BACStatus::Success:
		{
			//成功状态
			OutPutLog(__FUNCTION__, "send machine info success...");
			break;
		}
		default:
		{
			baclog->FileLog("base send machine info status error: " + std::to_string(this->TcpServer::GetResponseStatus(response)));
			return BASE_CLIENT_SEND_MACHINE_INFO_ERROR;
		}
	}

	return BASE_SUCCESS;
}

auto BACClient::SendAbnormalGameData(std::string abnormal_data) -> BASE_ERROR
{
#if NDEBUG
	VMProtectBeginUltra("BACClient::SendAbnormalGameData");
#endif

	//发送请求指令
	if (!this->TcpServer::Send(BACCommand::BAC_CLIENT_GAME_ABNORMAL_DATA, abnormal_data))
		return BASE_CLIENT_SEND_ABNORMAL_GAME_DATA_ERROR;

	//判断响应指令是否合法
	auto response = this->TcpServer::Recv(sizeof(BACPacketHeader));
	if (this->TcpServer::GetCommand(response) != BACCommand::BAC_CLIENT_GAME_ABNORMAL_DATA)
		return BASE_CLIENT_SEND_ABNORMAL_GAME_DATA_ERROR;

	//状态派遣
	switch (this->TcpServer::GetResponseStatus(response))
	{
		case BACStatus::Success:
		{
			OutPutLog(__FUNCTION__, "send abnormal game data success...");
			baclog->FileLog("send abnormal game data success...");
			break;
		}
		default:
		{
			OutPutLog(__FUNCTION__, "send abnormal game data status error: " + std::to_string(this->TcpServer::GetResponseStatus(response)));
			baclog->FileLog("send abnormal game data status error: " + std::to_string(this->TcpServer::GetResponseStatus(response)));
			return BASE_CLIENT_SEND_ABNORMAL_GAME_DATA_ERROR;
		}
	}



	return BASE_SUCCESS; 
#if NDEBUG
	VMProtectEnd();
#endif
}

