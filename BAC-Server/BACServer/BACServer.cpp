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

EnHandleResult BACServer::OnPrepareListen(ITcpServer* pSender, SOCKET soListen)
{
	return HR_OK;
}

EnHandleResult BACServer::OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient)
{
	PClientNode client = nullptr;

	std::cout << __FUNCTION__ << ": connect id: " << dwConnID << std::endl;

	try
	{
		//实例化一个客户端节点
		client = new ClientNode;

		client->server = pSender;
		client->connect_id = dwConnID;
		//client->login_time = this->Tools::GetSystemTime();

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

	std::cout << __FUNCTION__ << ": connect id: " << dwConnID << std::endl;

	if (pSender->GetConnectionExtra(dwConnID, (PVOID*)&client))
	{
		try
		{
			//获取客户端发来的数据并派遣
			PBACPacketHeader p_header = (PBACPacketHeader)data.data();
			std::cout << "command:" << p_header->cmd<< std::endl;
			std::cout << "status" << p_header->status << std::endl;
			std::cout << "data len:" << p_header->data_len << std::endl;

			//判断命令
			switch (p_header->cmd)
			{
				case BACCommand::BAC_CLIENT_LOGIN:
				{
					//填充登录响应头
					BACPacketHeader header = { 0,BACStatus::Success,p_header->cmd };
					std::string buffer((const char*)&header, sizeof(BACPacketHeader));


					client->server->Send(client->connect_id, (const BYTE*)buffer.data(), (int)buffer.size());
					break;
				}
				default:
					throw std::make_shared<BACServerError>("unknown packet status: " + std::to_string(p_header->status));
			}

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
	return HR_OK;
}

EnHandleResult BACServer::OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
{
	PClientNode client = nullptr;

	std::cout << __FUNCTION__ << ": connect id:" << dwConnID << std::endl;

	if (pSender->GetConnectionExtra(dwConnID, (PVOID*)&client))
	{
		try
		{
			//断开连接前的处理


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

