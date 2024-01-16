////////////////////////////////////////////////////////////////
//						待更新日志类
////////////////////////////////////////////////////////////////
#include "BACLog.h"


BACLog::BACLog()
{
}

BACLog::~BACLog()
{
}

void BACLog::OutPutCommandLine(const char* title, const char* data)
{
	printf("[BAC]:%s -> %s\n", title, data);
}

