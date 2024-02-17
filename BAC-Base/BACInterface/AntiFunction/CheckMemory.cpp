////////////////////////////////////////////////////////////////
//						在此检查Hook点
////////////////////////////////////////////////////////////////
#include "../BAC.h"
#include "../../BAC-Base.h"


bool BAC::JudgmentHookModule(PVOID hook_address)
{
#if NDEBUG
	VMProtectBeginUltra("BAC::JudgmentHookModule");
#endif

	DWORD64 bac_module_end = (DWORD64)this->Tools::GetModuleEndAddress(self_module);

	//计算jmp跳转到的地址
	DWORD64 target_address = ((DWORD64)hook_address + 5) + this->Tools::ReadInt((PVOID)((DWORD64)hook_address + 1));

#if _WIN64
	//判断是否有跳板
	if ((this->Tools::ReadByte((PVOID)target_address) == 0xFF) && (this->Tools::ReadByte((PVOID)(target_address + 1)) == 0x25))
	{
		target_address = this->Tools::ReadULong64((PVOID)(((DWORD64)target_address + 6) + this->Tools::ReadInt((PVOID)((DWORD64)target_address + 2))));
		if ((target_address >= (DWORD64)self_module) && (target_address <= bac_module_end))
			return true;
		else
		{
			std::cout << "[LOG]:" << __FUNCTION__ << " send to server detours hook had been changed" << std::endl;
			return false;
		}
	}
	else
	{
		std::cout << "[LOG]:" << __FUNCTION__ << " send to server is not BAC's detours lib hook" << std::endl;
		return false;
	}
#else
	if ((this->Tools::ReadByte((PVOID)target_address) == 0xE9))
	{
		target_address = ((DWORD64)target_address + 5) + this->Tools::ReadInt((PVOID)((DWORD64)target_address + 1));
		if ((target_address >= (DWORD64)self_module) && (target_address <= bac_module_end))
			return true;
		else
		{
			//std::cout << "[LOG]:" << __FUNCTION__ << " send to server detours hook had been changed" << std::endl;
			return false;
		}
	}
	else
	{
		//std::cout << "[LOG]:" << __FUNCTION__ << " send to server is not BAC's detours lib hook" << std::endl;
		return false;
	}
#endif

	return true;
#if NDEBUG
	VMProtectEnd();
#endif
}

void BAC::CheckHookPointer()
{
#if NDEBUG
	VMProtectBeginUltra("BAC::CheckHookPointer");
#endif

	for (auto pair : this->_hook_list)
	{
		for (auto tpair : pair.second)
		{
			//效验CRC32
			if (tpair.second != this->CRC32((void*)tpair.first, 5))
				std::cout << "[LOG]:" << __FUNCTION__ << " " << pair.first << " CRC32 value is error" << std::endl;

			//效验Hook点jmp后四位是否跳转到BAC模块中
			if (!this->JudgmentHookModule((void*)tpair.first))
				std::cout << "[LOG]:" << __FUNCTION__ << " hook to address not bac module!" << std::endl;
		}
	}

#if NDEBUG
	VMProtectEnd();
#endif
}

bool BAC::InitMemoryCRC32List()
{
	baclog->FunctionLog(__FUNCTION__, "Enter");

	//等待程序模块加载完成
	//::Sleep(180000);
	::Sleep(5000);

	//枚举进程模块信息<模块路径,模块地址>
	std::map<std::string, DWORD64> process_module_list;
	if (this->BAC::Tools::GetProcessModule(&process_module_list))
	{
		for (auto& pair : process_module_list)
		{
			PVOID image_handle = (PVOID)pair.second;

			DWORD64 text_address = (DWORD64)this->Tools::GetPeSectiontAddress(image_handle, ".text");
			DWORD text_size = this->Tools::GetPeSectionSize(image_handle, ".text");

			//向CRC32列表中插入元素
			this->_crc32_list[pair.first].emplace(text_address, text_size);
		}
	}
	else
	{
		//告诉服务器获取进程模块列表失败，或做一些其他处理
		baclog->FileLog("get process module list error!");
	}

	baclog->FunctionLog(__FUNCTION__, "Leave");
	return true;
}

void BAC::CheckMemoryCRC32()
{
#if NDEBUG
	VMProtectBeginUltra("BAC::CheckMemoryCRC32");
#endif
	baclog->FunctionLog(__FUNCTION__, "Enter");
	
	//初始化时的CRC32值列表<内存起始地址,CRC32值>
	static std::map<DWORD64, unsigned int> crc32_value_list;

	//初始化进程模块列表
	static bool first_call = false;
	if (!first_call)
	{
		this->InitMemoryCRC32List();

		//计算CRC32并保存初始值
		for (auto& pair : this->_crc32_list)
			for (auto& p : pair.second)
				crc32_value_list.insert(std::make_pair(p.first, this->CRC32((PVOID)p.first, p.second)));

		first_call = true;
	}

	//计算CRC32值并检查
	for (auto& pair : this->_crc32_list)
	{
		//printf("当前检查的模块：%s,", pair.first.c_str());
		for (auto& tpair : pair.second)
		{
			unsigned int crc32_value = this->CRC32((PVOID)tpair.first, tpair.second);
			//printf("地址：%p,CRC32值：%p\n", tpair.first, crc32_value);
			if (crc32_value != crc32_value_list[tpair.first])
			{
				//检测到当前模块被修改，可以dump并和本地文件一起上传服务器做数据回溯
				printf("检查到模块内存被修改：%s\n", pair.first.c_str());
			}
		}
	}

	baclog->FunctionLog(__FUNCTION__, "Leave");
#if NDEBUG
	VMProtectEnd();
#endif
}

