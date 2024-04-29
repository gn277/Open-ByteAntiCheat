#include "Direct.h"


DirectError::DirectError(std::string error) :_error_str(error)
{
}

DirectError::~DirectError()
{
}

char const* DirectError::what() const throw()
{
	return this->_error_str.c_str();
}


Direct9::Direct9()
{
}

Direct9::~Direct9()
{
}


Direct10::Direct10()
{
}

Direct10::~Direct10()
{
}


Direct11::Direct11()
{
}

Direct11::~Direct11()
{
}


Direct12::Direct12()
{
}

Direct12::~Direct12()
{
}


Direct::Direct()
{
}

Direct::~Direct()
{
}

