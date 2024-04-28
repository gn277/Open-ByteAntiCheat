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
#pragma warning(disable : 4996)
    return ExAllocatePoolWithTag(pool_type, size, 'bacb');
#pragma warning(default : 4996)
}

void BACBase::operator delete(void* pointer)
{
    ExFreePoolWithTag(pointer, 'bacb');
}

void BACBase::SetFileEventHandle(PKEVENT file_hanle)
{
    this->_file_event_handle = file_hanle;
}

PKEVENT BACBase::GetFileEventHandle()
{ 
    return this->_file_event_handle;
}

void BACBase::SetKernelThreadStatus(bool status)
{
    this->_kernel_loop_event_status = status;
}

bool BACBase::GetKernelThreadStatus()
{
    return this->_kernel_loop_event_status;
}


