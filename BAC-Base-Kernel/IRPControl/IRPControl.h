#pragma once
#include <wdm.h>
#include <ntdef.h>

#include "BAC-DataStruct.h"
#include "IRPControlCode.h"


NTSTATUS BACDispatchRoutine(PDEVICE_OBJECT device_object, PIRP irp);

