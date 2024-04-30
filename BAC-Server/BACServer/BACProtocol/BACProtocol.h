#pragma once

//BAC协议命令
enum BACCommand
{
	BAC_CLIENT_LOGIN = 10000001,							//BAC-Base登录命令
	BAC_CLIENT_MACHINE_INFO = 10000003,						//客户机器信息

	BAC_CLIENT_GAME_ABNORMAL_DATA = 20000000,				//游戏数据

	BAC_UNKNOWN = 99000099,									//未知命令
};

enum BACStatus
{
	None = 10000000,										//初始状态
	Request = 10000001,										//请求状态
	Response = 10000002,									//响应状态
	Transfer = 10000003,									//传输状态
	Error = 10000004,										//错误状态
	Success = 10000005,										//成功
	UNKNOWN = 10000006,										//未知状态
};

//BAC协议数据包头
typedef struct _BACPacketHeader
{
	unsigned long long data_len;							//数据包字节数总大小
	BACCommand cmd;											//执行命令参数
	BACStatus status = BACStatus::None;						//命令执行状态
}BACPacketHeader, * PBACPacketHeader;

typedef struct _BACPacket
{
	BACPacketHeader header;									//包头
	PVOID data;												//数据
}BACPacket, * PBACPacket;

