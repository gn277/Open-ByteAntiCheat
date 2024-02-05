////////////////////////////////////////////////////////////////
//						��������־��
////////////////////////////////////////////////////////////////
#include "BACLog.h"


BACLog::BACLog()
{
	//��ȡ��ǰʱ��
	time_t current_time;
	char ctime[50] = { NULL };

	time(&current_time);
	struct tm* local_time = localtime(&current_time);
	sprintf_s(ctime, "%d-%d-%d %02d-%02d-%02d.txt",
		(1900 + local_time->tm_year),
		(1 + local_time->tm_mon),
		local_time->tm_mday,
		local_time->tm_hour,
		local_time->tm_min,
		local_time->tm_sec);

	//��ȡ��ǰexe����Ŀ¼
	char current_path[MAX_PATH] = { NULL };
	::GetModuleFileNameA(NULL, current_path, MAX_PATH);
	strrchr(current_path, '\\')[0] = 0;					//ȥ��exe����
	this->_log_path.append(current_path);
	this->_log_path.append("\ByteAntiCheat\\BACLog\\");
	//this->_log_path.append("\BACLog\\");

	//�ж�BACLog�ļ����Ƿ���ڲ�����
	if (!std::filesystem::exists(this->_log_path))
		std::filesystem::create_directory(this->_log_path);

	//��־�ļ�·��
	this->_log_path.append(ctime);
}

BACLog::~BACLog()
{
}

void BACLog::OutPutCommandLine(const char* title, const char* data)
{
	printf("[BAC]:%s -> %s\n", title, data);
}

std::string BACLog::StringFromWideString(std::wstring wide_string)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

	std::string string = converter.to_bytes(wide_string);

	return string;
}

void BACLog::FileLog(std::string log_data)
{
	std::ofstream f(this->_log_path.c_str(), std::ofstream::out | std::ofstream::app);

	f << log_data.c_str() << std::endl;

	f.close();
}

void BACLog::FileLogf(const char* format, ...)
{
	char temp_string[8192];

	va_list va_arg_list;
	va_start(va_arg_list, format);
	vsprintf(temp_string, format, va_arg_list);
	va_end(va_arg_list);

	this->FileLog(temp_string);
}

void BACLog::FunctionLog(const char* function_name, const char* log_data)
{
	char temp_string[8192];

	sprintf_s(temp_string, "[BAC]-> %s: %s", function_name, log_data);

	this->FileLog(temp_string);
}



