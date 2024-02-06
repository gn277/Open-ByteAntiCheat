#include "BACTools.h"


BACTools::BACTools()
{
}

BACTools::~BACTools()
{
}

void* BACTools::operator new(size_t size, POOL_TYPE pool_type)
{
    return ExAllocatePoolWithTag(pool_type, size, 'bact');
}

void BACTools::operator delete(void* pointer)
{
    ExFreePoolWithTag(pointer, 'bact');
}

