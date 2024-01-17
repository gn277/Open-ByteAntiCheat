#pragma once
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <time.h>


class BACLog
{
private:
	std::string _log_path;

public:
	void OutPutCommandLine(const char* title, const char* data);
	void FileLog(std::string file_path, std::string log_data);
	void FileLogf(std::string szFileName, const char* c_szFormat, ...);

public:
	BACLog();
	~BACLog();

};

