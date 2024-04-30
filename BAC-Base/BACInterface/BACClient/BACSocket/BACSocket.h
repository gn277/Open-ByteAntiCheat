#pragma once
#include <stdio.h>
#include <iostream>
#include <winsock.h>

#include "../../BACError.h"
#include "../BACTools/BACTools.h"
#include "../../../../BAC-Server/BACServer/BACProtocol/BACProtocol.h"


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


class TcpServer :public BACTcpSocket, public BACTools
{
private:

public:
	TcpServer(int af = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP);
	~TcpServer();

public:
	auto Send(BACCommand cmd, std::string data = "", BACStatus status = BACStatus::Request) -> int;
	auto Recv(std::string* buffer, int recv_len) -> int;
	auto Recv(int recv_len) -> std::string;
	auto GetResponseStatus(std::string buffer) -> BACStatus;
	auto GetCommand(std::string buffer) -> BACCommand;
	auto GetDataLength(std::string buffer) -> unsigned long long;

public:

};

