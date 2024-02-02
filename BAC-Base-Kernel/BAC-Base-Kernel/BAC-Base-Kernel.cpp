#include "BAC-Base-Kernel.h"


void OutPutBACLog(IN const char* function, IN const char* data)
{
	DbgPrint("[BAC]:%s-> %s", function, data);
}

BACBase::BACBase(PDRIVER_OBJECT p_driver_object) :ProcessProtect(p_driver_object)
{
}

BACBase::~BACBase()
{
}

void* BACBase::operator new(size_t size, POOL_TYPE pool_type)
{
    return ExAllocatePoolWithTag(pool_type, size, 'bacb');
}

void BACBase::operator delete(void* pointer)
{
    ExFreePoolWithTag(pointer, 'bacb');
}



