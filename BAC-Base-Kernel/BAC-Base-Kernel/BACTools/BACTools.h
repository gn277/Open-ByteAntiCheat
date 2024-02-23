#pragma once
#include <ntifs.h>

#define DELAY_ONE_MICROSECOND (-10)
#define DELAY_ONE_MILLISECOND (DELAY_ONE_MICROSECOND*1000)


class BACTools
{
private:

public:
	void* operator new(size_t size, POOL_TYPE pool_type = NonPagedPool);
	void operator delete(void* pointer);

public:
	BACTools();
	~BACTools();

public:
	void Sleep(unsigned long msec);

};
