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

void BACTools::Sleep(unsigned long msec)
{
	LARGE_INTEGER my_interval;

	my_interval.QuadPart = DELAY_ONE_MILLISECOND;
	my_interval.QuadPart *= msec;

	KeDelayExecutionThread(KernelMode, 0, &my_interval);
}

