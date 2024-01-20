#pragma once
#include <wdm.h>
#include <ntdef.h>

#include "BAC-DataStruct.h"

//Í¨Ñ¶¿ØÖÆÂë
#define Test_Code CTL_CODE(FILE_DEVICE_UNKNOWN,0x801,METHOD_BUFFERED,FILE_ANY_ACCESS)


NTSTATUS BACDispatchRoutine(PDEVICE_OBJECT device_object, PIRP irp);

