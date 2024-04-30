#include "BACServer.h"


template<typename T1, typename T2>
void OutPutLog(T1 title, T2 data)
{
#if _PRINT_INFORMATION
	std::cout << "[BACServer]-> " << title << " : " << data << std::endl;
#else
	return;
#endif
}


BACServerError::BACServerError(const char* error) :_error_str(error)
{
}

BACServerError::BACServerError(std::string error) :_error_str(error)
{
}

BACServerError::~BACServerError()
{
}

char const* BACServerError::what() const throw()
{
	return this->_error_str.c_str();
}


BACServer::BACServer() :_tcp(this), _udp(this)
{
}

BACServer::~BACServer()
{
}

//TCP服务回调函数
EnHandleResult BACServer::OnPrepareListen(ITcpServer* pSender, SOCKET soListen)
{
	return HR_OK;
}

EnHandleResult BACServer::OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient)
{
	PClientNode client = nullptr;

	try
	{
		//实例化一个客户端节点
		client = new ClientNode;

		client->server = pSender;
		client->connect_id = dwConnID;
		//client->login_time = this->Tools::GetSystemTime();

		
		this->_clients.insert(std::make_pair(client->connect_id, client));
		pSender->SetConnectionExtra(dwConnID, client);
	}
	catch (const std::shared_ptr<BACServerError>& e)
	{
		OutPutLog(__FUNCTION__ + std::string(" had exception"), e->what());
	}
	catch (const std::exception& e)
	{
		OutPutLog(__FUNCTION__ + std::string(" had exception"), e.what());
	}

	return HR_OK;
}

EnHandleResult BACServer::OnHandShake(ITcpServer* pSender, CONNID dwConnID)
{
	return HR_IGNORE;
}

EnHandleResult BACServer::OnReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
	PClientNode client = nullptr;
	std::string data((char*)pData, iLength);

	if (pSender->GetConnectionExtra(dwConnID, (PVOID*)&client))
	{
		try
		{
			//后期可以在这里收到数据包后进行解密操作，再进行消息派遣

			this->DispatchTcpMessage(client, data);
		}
		catch (const std::shared_ptr<BACServerError>& e)
		{
			OutPutLog(__FUNCTION__ + std::string(" had exception"), e->what());
		}
		catch (const std::exception& e)
		{
			OutPutLog(__FUNCTION__ + std::string(" had exception"), e.what());
		}
	}

	return HR_OK;
}

EnHandleResult BACServer::OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
	//后期可以在数据包send前进入此函数进行数据包的加密操作

	return HR_OK;
}

EnHandleResult BACServer::OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
{
	PClientNode client = nullptr;

	if (pSender->GetConnectionExtra(dwConnID, (PVOID*)&client))
	{
		try
		{
			//断开连接前的处理

			OutPutLog("client leave", this->_clients[client->connect_id]->user_id);
			this->_clients.erase(client->connect_id);
			delete client;
		}
		catch (const std::shared_ptr<BACServerError>& e)
		{
			OutPutLog(__FUNCTION__ + std::string(" had exception"), e->what());
		}
		catch (const std::exception& e)
		{
			OutPutLog(__FUNCTION__ + std::string(" had exception"), e.what());
		}
	}

	return HR_OK;
}

EnHandleResult BACServer::OnShutdown(ITcpServer* pSender)
{
	return HR_OK;
}

CTcpServerPtr* BACServer::GetTcpServerPtr()
{
	return &this->_tcp;
}

//UDP服务回调函数
EnHandleResult BACServer::OnPrepareListen(IUdpNode* pSender, SOCKET soListen)
{
	//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
	//printf("[UDP]Start Success\n");
	//fflush(stdout);
	//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	return HR_OK;
}

EnHandleResult BACServer::OnSend(IUdpNode* pSender, LPCTSTR lpszRemoteAddress, USHORT usRemotePort, const BYTE* pData, int iLength)
{
	return HR_OK;
}

EnHandleResult BACServer::OnReceive(IUdpNode* pSender, LPCTSTR lpszRemoteAddress, USHORT usRemotePort, const BYTE* pData, int iLength)
{
	return HR_OK;
}

EnHandleResult BACServer::OnError(IUdpNode* pSender, EnSocketOperation enOperation, int iErrorCode, LPCTSTR lpszRemoteAddress, USHORT usRemotePort, const BYTE* pBuffer, int iLength)
{
	return HR_OK;
}

EnHandleResult BACServer::OnShutdown(IUdpNode* pSender)
{
	return HR_OK;
}

CUdpNodePtr* BACServer::GetUdpNodePtr()
{
	return &this->_udp;
}

SERVER_ERROR BACServer::StartTcpServer(const wchar_t* bind_address, unsigned short port)
{
	SERVER_ERROR status = SERVER_SUCCESS;

	if (!this->_tcp->Start(bind_address, port))
		return SERVER_START_TCP_ERROR;

	return status;
}

SERVER_ERROR BACServer::StartUdpServer()
{
	SERVER_ERROR status = SERVER_UNSUCCESSFUL;



	return status;
}

auto BACServer::GetClients() -> std::map<CONNID, PClientNode>
{
	return this->_clients;
}

auto BACServer::OnCommandError(PClientNode p_client, std::string buffer) -> void
{
	BACPacketHeader header = { 0,BACCommand::BAC_UNKNOWN,BACStatus::Error };
	std::string error_data((const char*)&header, sizeof(BACPacketHeader));

	p_client->server->Send(p_client->connect_id, (const BYTE*)error_data.data(), (int)error_data.size());
}

auto BACServer::OnStatusError(PClientNode p_client, std::string buffer) -> void
{
	PBACPacketHeader p_header = (PBACPacketHeader)buffer.data();
	BACPacketHeader header = { 0,p_header->cmd,BACStatus::Error };
	std::string error_data((const char*)&header, sizeof(BACPacketHeader));

	p_client->server->Send(p_client->connect_id, (const BYTE*)error_data.data(), (int)error_data.size());
}

auto BACServer::ResponseStatus(PClientNode p_client, BACCommand command, BACStatus status) -> bool
{
	BACPacketHeader header = { 0,command,status };
	std::string buffer((const char*)&header, sizeof(BACPacketHeader));

	return p_client->server->Send(p_client->connect_id, (const BYTE*)buffer.data(), (int)buffer.size());
}

auto BACServer::ClientLogin(PClientNode p_client, std::string buffer) -> void
{
	//获取客户端发来的数据并派遣
	PBACPacketHeader p_header = (PBACPacketHeader)buffer.data();

	switch (p_header->status)
	{
		case BACStatus::Request:
		{
			this->_clients[p_client->connect_id]->login = true;
			this->_clients[p_client->connect_id]->user_id = buffer.substr(sizeof(BACPacketHeader), p_header->data_len);

			OutPutLog("client login success", this->_clients[p_client->connect_id]->user_id);
			this->ResponseStatus(p_client, p_header->cmd);
			break;
		}
		default:
		{
			//登录状态不正确
			OutPutLog(__FUNCTION__, "client login status error" + std::to_string(p_header->status));
			this->OnStatusError(p_client, buffer);
			break;
		}
	}
}

auto BACServer::GetClientMachineInfo(PClientNode p_client, std::string buffer) -> void
{
	PBACPacketHeader p_header = (PBACPacketHeader)buffer.data();

	//判断命令状态
	switch (p_header->status)
	{
		case BACStatus::Request:
		{
			std::cout << "machine info: " << buffer.substr(sizeof(BACPacketHeader), p_header->data_len) << std::endl;

			//客户端请求时返回ok状态
			this->ResponseStatus(p_client, p_header->cmd);
			break;
		}
		default:
		{
			//获取机器信息时状态错误
			OutPutLog(__FUNCTION__, "get client machine info error: " + std::to_string(p_header->status));
			this->OnStatusError(p_client, buffer);
			break;
		}
	}
}

auto BACServer::GetClientAbnormalGameData(PClientNode p_client, std::string buffer) -> void
{
	PBACPacketHeader p_header = (PBACPacketHeader)buffer.data();

	//判断命令状态
	switch (p_header->status)
	{
		case BACStatus::Request:
		{
			std::cout << "user: " << this->_clients[p_client->connect_id]->user_id << " " << buffer.substr(sizeof(BACPacketHeader), p_header->data_len) << std::endl;

			this->ResponseStatus(p_client, p_header->cmd);
			break;
		}
		default:
		{
			OutPutLog(__FUNCTION__, "get client machine info error: " + std::to_string(p_header->status));
			this->OnStatusError(p_client, buffer);
			break;
		}
	}
}

auto BACServer::DispatchTcpMessage(PClientNode p_client, std::string buffer) -> void
{
	//获取客户端发来的数据并派遣
	PBACPacketHeader p_header = (PBACPacketHeader)buffer.data();
	//std::cout << "command:" << p_header->cmd<< std::endl;
	//std::cout << "status" << p_header->status << std::endl;
	//std::cout << "data len:" << p_header->data_len << std::endl;

	//判断命令(命令->状态->对应分支)
	switch (p_header->cmd)
	{
		case BACCommand::BAC_CLIENT_LOGIN:
		{
			this->ClientLogin(p_client, buffer);
			break;
		}
		case BACCommand::BAC_CLIENT_MACHINE_INFO:
		{
			this->GetClientMachineInfo(p_client, buffer);
			break;
		}
		case BACCommand::BAC_CLIENT_GAME_ABNORMAL_DATA:
		{
			this->GetClientAbnormalGameData(p_client, buffer);
			break;
		}
		default:
		{
			//未知命令
			this->OnCommandError(p_client, buffer);
			throw std::make_shared<BACServerError>("unknown packet status: " + std::to_string(p_header->cmd));
		}
	}
}

