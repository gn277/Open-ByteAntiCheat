////////////////////////////////////////////////////////////////
//				���ﶨ���ں���Ӧ�ò㹲�õ�ͨѶ������
////////////////////////////////////////////////////////////////
#pragma once

//ͨѶ������
#define ProtectProcess_Code CTL_CODE(FILE_DEVICE_UNKNOWN,0x801,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define MemoryProtect_Code CTL_CODE(FILE_DEVICE_UNKNOWN,0x802,METHOD_BUFFERED,FILE_ANY_ACCESS)

#define SEND_FILE_EVENT_HANDLE 1				//�ļ���д��Ϣ���

