#include "../DriverEntry.h"
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
				case ProtectProcess_Code:
				{
					PVOID buffer = (PVOID)irp->AssociatedIrp.SystemBuffer;
					*(LONG*)buffer = bac->ProcessProtect::ProtectProcess(((PProtectProcessStruct)buffer)->file_path);
					info = sizeof(LONG);
					break;
				}
				case MemoryProtect_Code:
				{
					PVOID buffer = (PVOID)irp->AssociatedIrp.SystemBuffer;
					*(LONG*)buffer = bac->MemoryProtect::RemapImage(((PProtectMemoryStruct)buffer)->module_name,
						((PProtectMemoryStruct)buffer)->process_id, ((PProtectMemoryStruct)buffer)->memory_address,
						((PProtectMemoryStruct)buffer)->memory_size);
					info = sizeof(LONG);
					break;
				}
			}
		}
		case IRP_MJ_CREATE:
			break;
		case IRP_MJ_CLOSE:
			break;
		case IRP_MJ_READ:
		{
			DbgPrint("[BAC]:MJ_READ\n");
			break;
		}
		case IRP_MJ_WRITE:
		{
			PPacketStruct p_packet = (PPacketStruct)irp->AssociatedIrp.SystemBuffer;

			//判断消息编号
			int message_number = p_packet->packet_number;
			if (message_number > 0)
			{
				switch (message_number)
				{
					case SEND_FILE_EVENT_HANDLE:
					{
						DbgPrint("[BAC]:内核收到消息同步事件\n");
						break;
					}
					default:
						break;
				}
			}
			else
			{
				//消息编号小于等于0
				status = STATUS_UNSUCCESSFUL;
			}

			info = stack->Parameters.Write.Length;
			break;
		}
	}
	irp->IoStatus.Information = info;
	irp->IoStatus.Status = status;
	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return status;
}

