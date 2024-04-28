#include "BAC.h"
#include "../BAC-Base.h"


BACError::BACError(const char* error) :_error_str(error)
{
}

BACError::BACError(std::string error) :_error_str(error)
{
}

BACError::~BACError()
{
}

char const* BACError::what() const throw()
{
	return this->_error_str.c_str();
}


BAC::BAC()
{
}

BAC::~BAC()
{
}

void BAC::AppendBACThreadHandle(std::string function_name, HANDLE thread_handle)
{
	this->bac_thread_list.insert(std::make_pair(function_name, thread_handle));
}

