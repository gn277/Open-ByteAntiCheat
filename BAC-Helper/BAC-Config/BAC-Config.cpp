#include "BAC-Config.h"


BACConfig::BACConfig()
{
	////获取当前exe运行目录
	//char current_path[MAX_PATH] = { NULL };
	//::GetModuleFileNameA(NULL, current_path, MAX_PATH);
	//strrchr(current_path, '\\')[0] = 0;					//去掉exe名称
	//this->_config_file_path.append(current_path);
	//this->_config_file_path.append("\\ByteAntiCheat\\BACConfig.ini");

	this->_config_file_path.append("ByteAntiCheat\\BACConfig.ini");

	//判断BACLog文件夹是否存在并创建
	if (!std::filesystem::exists(this->_config_file_path))
	{
		if (MessageBoxA(NULL, "BACConfig.ini file is missing, please check", "Error", MB_OK) == IDOK)
			exit(-1);
		else
			exit(-2);
	}

}

BACConfig::~BACConfig()
{
}

bool BACConfig::WriteConfig(std::string key, std::string value)
{
	//以追加方式打开文件
	FILE* file = fopen(this->_config_file_path.c_str(), "a+");
	//FILE* file = fopen(this->_config_file_path.c_str(), "r+");
	if (file == NULL)
	{
		if (MessageBoxA(NULL, "open BACConfig.ini file error,please check!", "Error", MB_OK) == IDOK)
			exit(-1);

		return false;
	}

	//先查找一次是否存在key
	//char line[256];
	//int current_line_number = 0;
	//while (fgets(line, sizeof(line), file) != NULL)
	//{
	//	//查找'='
	//	std::string temp_key(strtok(line, "="));
	//	if (!temp_key.empty())
	//	{
	//		if (temp_key.compare(key.c_str()) == 0)
	//		{
	//			std::cout << "找到行数：" << current_line_number << "执行修改" << std::endl;
	//
	//			char new_buffer[256] = { NULL };
	//			sprintf_s(new_buffer, "%s=%s\n", key.c_str(), value.c_str());
	//
	//			//定位到当前行的开头
	//			fseek(file, (long)(-(strlen(line))), SEEK_CUR);
	//			//写入新内容
	//			fputs(new_buffer, file);
	//
	//			fclose(file);
	//			return true;
	//		}
	//	}
	//	current_line_number++;
	//}

	//未找到key的情况
	fprintf(file, "%s=%s\n", key.c_str(), value.c_str());

	fclose(file);
	return true;
}

bool BACConfig::ReadConfig(std::string key, std::string* p_value)
{
	//只读方式打开，文件必须存在
	FILE* file = fopen(this->_config_file_path.c_str(), "r");
	if (file == NULL)
	{
		if (MessageBoxA(NULL, "open BACConfig.ini file error,please check!", "Error", MB_OK) == IDOK)
			exit(-1);

		return false;
	}

	char line[256];
	while (fgets(line, sizeof(line), file))
	{
		//查找'='
		std::string temp_key(strtok(line, "="));
		std::string temp_value(strtok(NULL, "\n"));
		if (!temp_key.empty() && !temp_value.empty())
		{
			if (temp_key.compare(key) == 0)
				p_value->append(temp_value.data());
		}
	}

	fclose(file);

	return true;
}



