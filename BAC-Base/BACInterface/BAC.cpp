#include "BAC.h"
#include "../BAC-Base.h"


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

