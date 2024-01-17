#pragma once
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <time.h>
#include <filesystem>

#include <utility>
#include <codecvt>


class BACLog
{
private:
	std::filesystem::path _log_path;

private:
	std::string StringFromWideString(std::wstring wide_string);

public:
	void OutPutCommandLine(const char* title, const char* data);
	void FileLog(std::string log_data);
	void FileLogf(const char* format, ...);
	void FunctionLog(const char* function_name, const char* log_data);

public:
	BACLog();
	~BACLog();

};

