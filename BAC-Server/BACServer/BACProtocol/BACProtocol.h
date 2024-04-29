#pragma once

//BAC协议命令
enum BACCommand
{
	BAC_CLIENT_LOGIN = 0x10000001,							//BAC-Base登录命令
};

enum BACStatus
{
	None = 0x01000000,										//初始状态
	Request = 0x01000001,									//请求状态
	Response = 0x01000002,									//响应状态
	Transfer = 0x01000003,									//传输状态
	Error = 0x01000004,										//错误状态
	Success = 0x01000005,									//成功
	UNKNOWN = 0x01000006,									//未知状态
};

//BAC协议数据包头
typedef struct _BACPacketHeader
{
	unsigned long long data_len;							//数据包字节数总大小
	BACStatus status = BACStatus::None;						//初始状态
	BACCommand cmd;											//执行命令参数
}BACPacketHeader, * PBACPacketHeader;

