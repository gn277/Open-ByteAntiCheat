#include <wdm.h>


extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT driver_object, PUNICODE_STRING register_path)
{
	NTSTATUS status = STATUS_SUCCESS;



	DbgPrint("[BAC]:BAC driver initialize...");
	return status;
}