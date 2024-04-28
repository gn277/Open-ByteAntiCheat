#include "BAC.h"
#include "../BAC-Base.h"


std::shared_ptr<BAC> bac = nullptr;
std::shared_ptr<BACClient> client = nullptr;
std::shared_ptr<BACLog> baclog = nullptr;


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


BAC::BAC(HMODULE self_module_handle) :_self_module(self_module_handle)
{
}

BAC::~BAC()
{
}

void BAC::AppendBACThreadHandle(std::string function_name, HANDLE thread_handle)
{
	this->bac_thread_list.insert(std::make_pair(function_name, thread_handle));
}

HMODULE BAC::GetSelfModuleHandle()
{
	return this->_self_module;
}

