#pragma once

//BACЭ������
enum BACCommand
{
	BAC_CLIENT_LOGIN = 0x10000001,							//BAC-Base��¼����
};

enum BACStatus
{
	None = 0x01000000,										//��ʼ״̬
	Request = 0x01000001,									//����״̬
	Response = 0x01000002,									//��Ӧ״̬
	Transfer = 0x01000003,									//����״̬
	Error = 0x01000004,										//����״̬
	Success = 0x01000005,									//�ɹ�
	UNKNOWN = 0x01000006,									//δ֪״̬
};

//BACЭ�����ݰ�ͷ
typedef struct _BACPacketHeader
{
	unsigned long long data_len;							//���ݰ��ֽ����ܴ�С
	BACStatus status = BACStatus::None;						//��ʼ״̬
	BACCommand cmd;											//ִ���������
}BACPacketHeader, * PBACPacketHeader;

