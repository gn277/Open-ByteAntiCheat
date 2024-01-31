#include "BAC-Config.h"


BACConfig::BACConfig()
{
	////��ȡ��ǰexe����Ŀ¼
	//char current_path[MAX_PATH] = { NULL };
	//::GetModuleFileNameA(NULL, current_path, MAX_PATH);
	//strrchr(current_path, '\\')[0] = 0;					//ȥ��exe����
	//this->_config_file_path.append(current_path);
	//this->_config_file_path.append("\\ByteAntiCheat\\BACConfig.ini");

	this->_config_file_path.append("ByteAntiCheat\\BACConfig.ini");

	//�ж�BACLog�ļ����Ƿ���ڲ�����
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
	//��׷�ӷ�ʽ���ļ�
	FILE* file = fopen(this->_config_file_path.c_str(), "a+");
	//FILE* file = fopen(this->_config_file_path.c_str(), "r+");
	if (file == NULL)
	{
		if (MessageBoxA(NULL, "open BACConfig.ini file error,please check!", "Error", MB_OK) == IDOK)
			exit(-1);

		return false;
	}

	//�Ȳ���һ���Ƿ����key
	//char line[256];
	//int current_line_number = 0;
	//while (fgets(line, sizeof(line), file) != NULL)
	//{
	//	//����'='
	//	std::string temp_key(strtok(line, "="));
	//	if (!temp_key.empty())
	//	{
	//		if (temp_key.compare(key.c_str()) == 0)
	//		{
	//			std::cout << "�ҵ�������" << current_line_number << "ִ���޸�" << std::endl;
	//
	//			char new_buffer[256] = { NULL };
	//			sprintf_s(new_buffer, "%s=%s\n", key.c_str(), value.c_str());
	//
	//			//��λ����ǰ�еĿ�ͷ
	//			fseek(file, (long)(-(strlen(line))), SEEK_CUR);
	//			//д��������
	//			fputs(new_buffer, file);
	//
	//			fclose(file);
	//			return true;
	//		}
	//	}
	//	current_line_number++;
	//}

	//δ�ҵ�key�����
	fprintf(file, "%s=%s\n", key.c_str(), value.c_str());

	fclose(file);
	return true;
}

bool BACConfig::ReadConfig(std::string key, std::string* p_value)
{
	//ֻ����ʽ�򿪣��ļ��������
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
		//����'='
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



