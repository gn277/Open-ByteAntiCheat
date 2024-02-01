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

void* BACBase::operator new(size_t size, POOL_TYPE pool_type)
{
    return ExAllocatePool2(pool_type, size, 'bac1');
}

void BACBase::operator delete(void* pointer)
{
    ExFreePoolWithTag(pointer, 'bac1');
}



