#include "BAC-Config.h"


BACConfig::BACConfig()
{
	////获取当前exe运行目录
	//char current_path[MAX_PATH] = { NULL };
	//::GetModuleFileNameA(NULL, current_path, MAX_PATH);
	//strrchr(current_path, '\\')[0] = 0;					//去掉exe名称
	//this->_config_file_path.append(current_path);
	//this->_config_file_path.append("\\ByteAntiCheat\\BACConfig.ini");

	////判断BACLog文件夹是否存在并创建
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



