#pragma once
#include <wdm.h>


class ProcessProtect
{
private:

public:
	ProcessProtect();
	~ProcessProtect();

public:
	void* operator new(size_t size, POOL_TYPE pool_type = NonPagedPool);
	void operator delete(void* pointer);

public:

};

