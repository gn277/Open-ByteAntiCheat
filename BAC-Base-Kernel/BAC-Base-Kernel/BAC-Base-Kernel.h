#pragma once
#include <ntifs.h>
#include <ntdef.h>
#include <ntddk.h>
#include <windef.h>
#include <ntimage.h>
#include <wdf.h>
#include <wdm.h>
#include <ntstrsafe.h>

#pragma comment(lib, "NtStrSafe.lib")
#pragma comment(lib, "NtosKrnl.lib")


void OutPutBACLog(IN const char* function, IN const char* data);

class BACBase
{
public:
	BACBase();
	~BACBase();

private:

};

