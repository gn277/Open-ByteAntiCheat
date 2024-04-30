#pragma once
#include <stdio.h>
#include <iostream>

#include "BACSocket/BACSocket.h"
#include <cJSON.h>


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

	//TCP业务处理函数：
private:

public:
	auto ConnectTcpServer(const char* host, int port) -> BASE_ERROR;
	auto Login(std::string user_id) -> BASE_ERROR;
	auto SendMachineInfo() -> BASE_ERROR;
	auto SendAbnormalGameData(std::string abnormal_data) -> BASE_ERROR;

	//UDP业务处理函数：
private:

public:

};

