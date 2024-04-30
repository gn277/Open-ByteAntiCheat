#pragma once

//BACЭ������
enum BACCommand
{
	BAC_CLIENT_LOGIN = 10000001,							//BAC-Base��¼����
	BAC_CLIENT_MACHINE_INFO = 10000003,						//�ͻ�������Ϣ

	BAC_CLIENT_GAME_ABNORMAL_DATA = 20000000,				//��Ϸ����

	BAC_UNKNOWN = 99000099,									//δ֪����
};

enum BACStatus
{
	None = 10000000,										//��ʼ״̬
	Request = 10000001,										//����״̬
	Response = 10000002,									//��Ӧ״̬
	Transfer = 10000003,									//����״̬
	Error = 10000004,										//����״̬
	Success = 10000005,										//�ɹ�
	UNKNOWN = 10000006,										//δ֪״̬
};

//BACЭ�����ݰ�ͷ
typedef struct _BACPacketHeader
{
	unsigned long long data_len;							//���ݰ��ֽ����ܴ�С
	BACCommand cmd;											//ִ���������
	BACStatus status = BACStatus::None;						//����ִ��״̬
}BACPacketHeader, * PBACPacketHeader;

typedef struct _BACPacket
{
	BACPacketHeader header;									//��ͷ
	PVOID data;												//����
}BACPacket, * PBACPacket;

