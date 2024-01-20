#include "IRPControl.h"


NTSTATUS BACDispatchRoutine(PDEVICE_OBJECT device_object, PIRP irp)
{
	ULONG info = 0;
	NTSTATUS status = STATUS_SUCCESS;
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(irp);

	//区分不同的IRP
	switch (stack->MajorFunction)
	{
		case IRP_MJ_DEVICE_CONTROL:
		{
			ULONG IrpCode = stack->Parameters.DeviceIoControl.IoControlCode;
			switch (IrpCode)
			{
				case Test_Code:
				{
					PVOID buffer = (PVOID)irp->AssociatedIrp.SystemBuffer;
					DWORD64 data = 0xFF;
					*(DWORD64*)buffer = (DWORD64)data;
					info = sizeof(DWORD64);
					break;
				}
			}
		}
		case IRP_MJ_CREATE:
			break;
		case IRP_MJ_CLOSE:
			break;
		case IRP_MJ_READ:
			break;
	}
	irp->IoStatus.Information = info;//当 IRP 引发数据的传送操作，通常设置 Information 值为传送的字节数。
	irp->IoStatus.Status = status;
	IoCompleteRequest(irp, IO_NO_INCREMENT);//指令完成派遣函数
	return status;
}


