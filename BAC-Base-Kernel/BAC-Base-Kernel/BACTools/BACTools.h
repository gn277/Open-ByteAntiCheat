#pragma once
#include <ntifs.h>


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

};
