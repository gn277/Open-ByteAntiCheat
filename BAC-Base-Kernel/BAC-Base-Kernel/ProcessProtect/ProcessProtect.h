#pragma once
#include <wdm.h>


class ProcessProtect
{
private:

private:
	__int64 ExpLookupHandleTableEntry(unsigned int* a1, __int64 a2);

public:
	ProcessProtect();
	~ProcessProtect();

public:
	void* operator new(size_t size, POOL_TYPE pool_type = NonPagedPool);
	void operator delete(void* pointer);

public:

};

