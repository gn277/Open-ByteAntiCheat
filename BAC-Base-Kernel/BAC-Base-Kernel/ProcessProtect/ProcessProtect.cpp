#include "ProcessProtect.h"


ProcessProtect::ProcessProtect()
{
}

ProcessProtect::~ProcessProtect()
{
}

void* ProcessProtect::operator new(size_t size, POOL_TYPE pool_type)
{
    return ExAllocatePool2(pool_type, size, 'bac1');
}

void ProcessProtect::operator delete(void* pointer)
{
    ExFreePoolWithTag(pointer, 'bac1');
}

