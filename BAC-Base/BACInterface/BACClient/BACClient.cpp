#include "BACClient.h"


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

BASE_ERROR BACClient::ConnectTcpServer(const char* host, int port)
{
	//连接TCP服务器
	auto result = this->TcpServer::BACTcpSocket::Connect(AF_INET, host, port);
	if (result != true)
		return BASE_CONNECT_TCP_SERVER_ERROR;

	return BASE_SUCCESS;
}

