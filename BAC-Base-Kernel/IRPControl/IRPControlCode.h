////////////////////////////////////////////////////////////////
//				这里定义内核与应用层共用的通讯控制码
////////////////////////////////////////////////////////////////
#pragma once

//通讯控制码
#define ProtectProcess_Code CTL_CODE(FILE_DEVICE_UNKNOWN,0x801,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define MemoryProtect_Code CTL_CODE(FILE_DEVICE_UNKNOWN,0x802,METHOD_BUFFERED,FILE_ANY_ACCESS)

#define SEND_FILE_EVENT_HANDLE 1				//文件读写消息编号

