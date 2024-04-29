#include "BACSocket.h"
#include "../../BAC.h"


BACTcpSocket::BACTcpSocket(int af, int type, int protocol)
{
	WSADATA wsa_data;

	//初始化wsa
	auto result = ::WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (result != 0)
	{
		std::cout << "wsa startup error" << std::endl;
		return;
	}

	//创建socket
	this->_socket = ::socket(af, type, protocol);

}

BACTcpSocket::~BACTcpSocket()
{	
	//关闭socket
	if (this->_socket)
		closesocket(this->_socket);

	::WSACleanup();

}

int BACTcpSocket::Connect(int af, const char* host, int port)
{
	//先判断构造函数中socket是否创建成功，失败返回 2
	if (!this->_socket)
		return 2;

	//连接服务器，失败返回 3
	SOCKADDR_IN address = { NULL };
	address.sin_family = af;
	address.sin_port = htons(port);
	address.sin_addr.S_un.S_addr = inet_addr(host);

	auto result = ::connect(this->_socket, (sockaddr*)&address, sizeof(address));
	if (result == -1)
	{
		std::cout << "connect server error:" << std::hex << ::GetLastError() << std::endl;
		return 3;
	}

	//成功返回 1
	return true;
}

bool BACTcpSocket::Listen()
{
	//监听端口，失败返回2
	auto result = ::listen(this->_socket, 10);
	if (result == -1)
	{
		std::cout << "listen port error: " << ::GetLastError() << std::endl;

		closesocket(this->_socket);
		::WSACleanup();
		return false;
	}

	return true;
}

SOCKET BACTcpSocket::Accept(sockaddr FAR* addr, int FAR* addrlen)
{
	return ::accept(this->_socket, addr, addrlen);
}

int BACTcpSocket::Recv(char FAR* buf, int len)
{
	auto readed = ::recv(this->_socket, buf, len, 0);
	return readed;
}

int BACTcpSocket::Send(const char FAR* buf, int len)
{
	auto sended = ::send(this->_socket, buf, len, 0);
	return sended;
}


TcpServer::TcpServer(int af, int type, int protocol) :BACTcpSocket(af, type, protocol)
{
}

TcpServer::~TcpServer()
{
}

auto TcpServer::Send(BACCommand cmd, std::string data, BACStatus status) -> int
{
	//添加包头信息
	BACPacketHeader header = { NULL };
	header.data_len = data.size();
	header.status = status;
	header.cmd = cmd;

	//将header和数据追加到buffer中
	std::string buffer((const char*)&header, sizeof(BACPacketHeader));
	buffer.append(data);

	return this->BACTcpSocket::Send(buffer.data(), buffer.size());
}

auto TcpServer::Recv(std::string* buffer, int recv_len) -> int
{
	buffer->clear();
	char* temp = new char[recv_len];

	auto recved = this->BACTcpSocket::Recv(temp, recv_len);

	buffer->append(temp, recved);
	delete[] temp;

	return recved;
}

auto TcpServer::GetStatus(std::string buffer) -> BACStatus
{
	PBACPacketHeader p_header = (PBACPacketHeader)buffer.data();

	return p_header->status;
}

auto TcpServer::GetCommand(std::string buffer) -> BACCommand
{
	PBACPacketHeader p_header = (PBACPacketHeader)buffer.data();

	return p_header->cmd;
}

auto TcpServer::GetData(std::string buffer) -> PVOID
{
	return buffer.substr(sizeof(BACPacketHeader)).data();
}

auto TcpServer::Login() -> BASE_ERROR
{
	baclog->FunctionLog(__FUNCTION__, "Enter");

	//发送登录命令
	if (!this->Send(BAC_CLIENT_LOGIN, ""))
	{
		baclog->FunctionLog(__FUNCTION__, "Leave");
		return BASE_CLIENT_LOGIN_ERROR;
	}

	//接收返回命令
	std::string buffer;
	if (!this->Recv(&buffer, sizeof(BACPacketHeader)))
	{
		baclog->FunctionLog(__FUNCTION__, "Leave");
		return BASE_CLIENT_LOGIN_ERROR;
	}

	//判断返回命令
	PBACPacketHeader p_header = (PBACPacketHeader)buffer.data();
	if (p_header->status != BACStatus::Success)
	{
		baclog->FileLogf("BASE login status error:%d", p_header->status);
		baclog->FunctionLog(__FUNCTION__, "Leave");
		return BASE_CLIENT_LOGIN_ERROR;
	}

	baclog->FunctionLog(__FUNCTION__, "Leave");
	return BASE_SUCCESS;
}

