#include "BAC-Config.h"


BACConfig::BACConfig()
{
	////��ȡ��ǰexe����Ŀ¼
	//char current_path[MAX_PATH] = { NULL };
	//::GetModuleFileNameA(NULL, current_path, MAX_PATH);
	//strrchr(current_path, '\\')[0] = 0;					//ȥ��exe����
	//this->_config_file_path.append(current_path);
	//this->_config_file_path.append("\\ByteAntiCheat\\BACConfig.ini");

	////�ж�BACLog�ļ����Ƿ���ڲ�����
	//if (!std::filesystem::exists(this->_config_file_path))
	//{
	//	MessageBoxA(NULL, "BACConfig.ini file is missing, please check", "Error", MB_OK);

	//}
	//else
	//{
	//	MessageBoxA(NULL, "BACConfig.ini file is exists", "ok", MB_OK);
	//}

}

BACConfig::~BACConfig()
{
}



