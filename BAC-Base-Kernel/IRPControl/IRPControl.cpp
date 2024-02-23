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
				case ClearDebugPort_Code:
				{
					PVOID buffer = (PVOID)irp->AssociatedIrp.SystemBuffer;
					*(LONG*)buffer = bac->ProcessProtect::ClearDebugPort(*(HANDLE*)buffer);
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
			//这里是应用层调用ReadFile后内核层发送数据到Ring3
			DbgPrint("[BAC]:MJ_READ\n");
			break;
		}
		case IRP_MJ_WRITE:
		{
			//这里是应用层调用WirteFile后内核层收到Ring3数据
			PPacketStruct p_packet = (PPacketStruct)irp->AssociatedIrp.SystemBuffer;

			int message_number = p_packet->packet_number;
			if (message_number > 0)
			{
				switch (message_number)
				{
					case SEND_FILE_EVENT_HANDLE:
					{
						DbgPrint("[BAC]:handle:%p", (HANDLE)p_packet->buffer);
						//初始化循环事件,在循环事件中调用KeSetEvent通知应用层来读取数据
						status = bac->InitializeKernelLoopEvent((HANDLE)p_packet->buffer);
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

