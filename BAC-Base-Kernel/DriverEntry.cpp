#include "DriverEntry.h"
#include "IRPControl/IRPControl.h"

BACBase* bac = nullptr;

void DriverUnload(PDRIVER_OBJECT p_driver_object)
{
#if _DEBUG
	OutPutBACLog(__FUNCTION__, "Enter");
#endif

	//ɾ���豸����
	PDEVICE_OBJECT p_device = p_driver_object->DeviceObject;
	IoDeleteDevice(p_device);
	//ɾ����������
	UNICODE_STRING link_name;
	RtlInitUnicodeString(&link_name, DRIVER_LINKER_NAME);
	IoDeleteSymbolicLink(&link_name);

	//�ͷ�BACBase
	if (bac)
		delete bac;

#if _DEBUG
	OutPutBACLog(__FUNCTION__, "Leave");
#endif
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT p_driver_object, PUNICODE_STRING p_register_path)
{
#if _DEBUG
	OutPutBACLog(__FUNCTION__, "Enter");
#endif

	NTSTATUS status = STATUS_SUCCESS;

	//ע����ǲ����
	p_driver_object->MajorFunction[IRP_MJ_CREATE] = BACDispatchRoutine;
	p_driver_object->MajorFunction[IRP_MJ_CLOSE] = BACDispatchRoutine;
	p_driver_object->MajorFunction[IRP_MJ_READ] = BACDispatchRoutine;
	p_driver_object->MajorFunction[IRP_MJ_WRITE] = BACDispatchRoutine;
	p_driver_object->MajorFunction[IRP_MJ_DEVICE_CONTROL] = BACDispatchRoutine;

	//���������豸
	PDEVICE_OBJECT p_device;
	UNICODE_STRING device_name;
	RtlInitUnicodeString(&device_name, DRIVER_NAME);
	status = IoCreateDevice(p_driver_object, 0, &device_name, FILE_DEVICE_UNKNOWN, 0, TRUE, &p_device);
	if (!NT_SUCCESS(status))
	{
		if (status == STATUS_INSUFFICIENT_RESOURCES)
			OutPutBACLog(__FUNCTION__, "��Դ����");
		if (status == STATUS_OBJECTID_EXISTS)
			OutPutBACLog(__FUNCTION__, "ָ���������Ѵ���");
		if (status == STATUS_OBJECT_NAME_COLLISION)
			OutPutBACLog(__FUNCTION__, "�������г�ͻ");
		OutPutBACLog(__FUNCTION__, "�����豸ʧ��");
		return status;
	}

	//������������
	p_driver_object->Flags |= DO_BUFFERED_IO;
	UNICODE_STRING link_name;
	RtlInitUnicodeString(&link_name, DRIVER_LINKER_NAME);
	status = IoCreateSymbolicLink(&link_name, &device_name);
	if (!NT_SUCCESS(status))
	{
		IoDeleteDevice(p_device);
		OutPutBACLog(__FUNCTION__, "�豸����ʧ�ܺ�ɾ���豸�ɹ�");
		return status;
	}
	p_driver_object->DriverUnload = DriverUnload;

	//ʵ����BACBase
	bac = new BACBase;

#if _DEBUG
	OutPutBACLog(__FUNCTION__, "Leave");
#endif
	return status;
}