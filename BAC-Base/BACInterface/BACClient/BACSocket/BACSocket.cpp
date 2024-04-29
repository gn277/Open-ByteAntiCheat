#include "BACSocket.h"


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
	address.sin_addr.S_un.S_addr = ntohl(inet_addr(host));

	auto result = ::connect(this->_socket, (sockaddr*)&address, sizeof(address));
	if (result == -1)
	{
		std::cout << "connect server error:" << ::GetLastError() << std::endl;
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

