#pragma once
#include <stdio.h>
#include <iostream>

#include "BACSocket/BACSocket.h"


class BACClientError :public std::exception
{
private:
	std::string _error_str;

public:
	BACClientError(const char* error);
	BACClientError(std::string error);
	~BACClientError();

public:
	virtual char const* what() const throw();

};


class BACClient :public TcpServer
{
private:

public:
	BACClient();
	~BACClient();

public:
	BASE_ERROR ConnectTcpServer(const char* host, int port);

};

