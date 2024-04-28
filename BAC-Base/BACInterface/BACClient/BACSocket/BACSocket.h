#pragma once
#include <stdio.h>
#include <iostream>
#include <winsock.h>

#include "../../BACError.h"


class BACTcpSocket
{
private:
	SOCKET _socket = NULL;

public:
	BACTcpSocket(int af = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP);
	~BACTcpSocket();

public:
	int Connect(int af, const char* host, int port);
	bool Listen();
	SOCKET Accept(sockaddr FAR* addr, int FAR* addrlen);
	int Recv(char FAR* buf, int len);
	int Send(const char FAR* buf, int len);

};


class TcpServer :public BACTcpSocket
{
private:

public:
	TcpServer(int af = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP);
	~TcpServer();

public:
	BASE_ERROR ConnectServer(const char* host, int port);

};

