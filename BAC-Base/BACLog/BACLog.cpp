////////////////////////////////////////////////////////////////
//						待更新日志类
////////////////////////////////////////////////////////////////
#include "BACLog.h"


BACLog::BACLog()
{
	printf("log...\n");
	//获取当前时间
	time_t current_time;
	struct tm* local_time = localtime(&current_time);
	char ctime[50] = { NULL };
	//sprintf_s(ctime, "%d-%d-%d %02d:%02d:%02d",
	//	(1900 + local_time->tm_year),
	//	(1 + local_time->tm_mon),
	//	local_time->tm_mday,
	//	local_time->tm_hour,
	//	local_time->tm_min,
	//	local_time->tm_sec);
	printf("%d-%d-%d %02d:%02d:%02d",
		(1900 + local_time->tm_year),
		(1 + local_time->tm_mon),
		local_time->tm_mday,
		local_time->tm_hour,
		local_time->tm_min,
		local_time->tm_sec);
	//printf("当前时间：%s\n", ctime);


}

BACLog::~BACLog()
{
}

void BACLog::OutPutCommandLine(const char* title, const char* data)
{
	printf("[BAC]:%s -> %s\n", title, data);
}

void BACLog::FileLog(std::string file_path, std::string log_data)
{
	std::ofstream f(file_path.c_str(), std::ofstream::out | std::ofstream::app);

	f << log_data.c_str() << std::endl;

	f.close();
}

void BACLog::FileLogf(std::string szFileName, const char* c_szFormat, ...)
{
	char cTmpString[8192];
	va_list vaArgList;
	va_start(vaArgList, c_szFormat);
	vsprintf(cTmpString, c_szFormat, vaArgList);
	va_end(vaArgList);

	this->FileLog(szFileName.c_str(), cTmpString);
}



