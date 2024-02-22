#pragma once
#include <ntifs.h>
#include <ntdef.h>
#include <ntddk.h>
#include <windef.h>
#include <ntimage.h>
#include <wdf.h>
#include <ntstrsafe.h>

#pragma comment(lib, "NtStrSafe.lib")
#pragma comment(lib, "NtosKrnl.lib")

#include "../../BAC-Base/ProtectSDK/VMProtectSDK/VMProtectSDK.h"

#include "UnDocumentedApi.h"
#include "BACTools/BACTools.h"
#include "ProcessProtect/ProcessProtect.h"
#include "MemoryProtect/MemoryProtect.h"


void OutPutBACLog(IN const char* function, IN const char* data);

class BACBase : public BACTools, public ProcessProtect, public MemoryProtect
{
private:

public:
	BACBase(PDRIVER_OBJECT p_driver_object);
	~BACBase();

public:
	void* operator new(size_t size, POOL_TYPE pool_type = NonPagedPool);
	void operator delete(void* pointer);

public:


};

