#include "BAC-Base-Kernel.h"


void OutPutBACLog(IN const char* function, IN const char* data)
{
	DbgPrint("[BAC]:%s-> %s", function, data);
}

BACBase::BACBase()
{
}

BACBase::~BACBase()
{
}

