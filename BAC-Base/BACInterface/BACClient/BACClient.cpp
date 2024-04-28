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

