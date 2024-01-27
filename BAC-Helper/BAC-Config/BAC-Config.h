#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <istream>
#include <ostream>
#include <filesystem>

using namespace std;


class BACConfig
{
private:
	std::string _config_file_path;

public:
	BACConfig();
	~BACConfig();

public:
	bool WriteConfig(std::string key, std::string value);
	bool ReadConfig(std::string key, std::string* p_value);

};

