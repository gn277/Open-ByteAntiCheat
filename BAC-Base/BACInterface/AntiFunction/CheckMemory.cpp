////////////////////////////////////////////////////////////////
//						�ڴ˼��Hook��
////////////////////////////////////////////////////////////////
#include "../BAC.h"


bool BAC::JudgmentHookModule(PVOID hook_address)
{
//#if NDEBUG
//	VMProtectBeginUltra("BAC::JudgmentHookModule");
//#endif

	DWORD64 bac_module_end = (DWORD64)this->BACTools::GetModuleEndAddress(bac->GetSelfModuleHandle());

	//����jmp��ת���ĵ�ַ
	DWORD64 target_address = ((DWORD64)hook_address + 5) + this->BACTools::ReadInt((PVOID)((DWORD64)hook_address + 1));

#if _WIN64
	//�ж��Ƿ�������
	if ((this->BACTools::ReadByte((PVOID)target_address) == 0xFF) && (this->BACTools::ReadByte((PVOID)(target_address + 1)) == 0x25))
	{
		target_address = this->BACTools::ReadULong64((PVOID)(((DWORD64)target_address + 6) + this->BACTools::ReadInt((PVOID)((DWORD64)target_address + 2))));
		if ((target_address >= (DWORD64)bac->GetSelfModuleHandle()) && (target_address <= bac_module_end))
			return true;
		else
		{
			std::cout << "[LOG]:" << __FUNCTION__ << " send to server detours hook had been changed" << std::endl;

			client->SendAbnormalGameData("detours hook had been changed");
			return false;
		}
	}
	else
	{
		std::cout << "[LOG]:" << __FUNCTION__ << " send to server is not BAC's detours lib hook" << std::endl;

		client->SendAbnormalGameData("find not BAC's detours lib hook");
		return false;
	}
#else
	if ((this->BACTools::ReadByte((PVOID)target_address) == 0xE9))
	{
		target_address = ((DWORD64)target_address + 5) + this->BACTools::ReadInt((PVOID)((DWORD64)target_address + 1));
		if ((target_address >= (DWORD64)bac->GetSelfModuleHandle()) && (target_address <= bac_module_end))
			return true;
		else
		{
			//std::cout << "[LOG]:" << __FUNCTION__ << " send to server detours hook had been changed" << std::endl;

			client->SendAbnormalGameData("detours hook had been changed");
			return false;
		}
	}
	else
	{
		//std::cout << "[LOG]:" << __FUNCTION__ << " send to server is not BAC's detours lib hook" << std::endl;

		client->SendAbnormalGameData("find not BAC's detours lib hook"");
		return false;
	}
#endif

	return true;
//#if NDEBUG
//	VMProtectEnd();
//#endif
}

void BAC::CheckHookPointer()
{
#if NDEBUG
	VMProtectBeginUltra("BAC::CheckHookPointer");
#endif

	for (auto api_name : this->_hook_list)
	{
		for (auto api_address : api_name.second)
		{
			//Ч��CRC32
			if (api_address.second != this->CRC32((void*)api_address.first, 5))
			{
				std::cout << "[LOG]:" << __FUNCTION__ << " " << api_name.first << " CRC32 value is error" << std::endl;
				client->SendAbnormalGameData(api_name.first + " CRC32 value is error");
			}

			//Ч��Hook��jmp����λ�Ƿ���ת��BACģ����
			if (!this->JudgmentHookModule((void*)api_address.first))
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

	//�ȴ�����ģ��������
	//::Sleep(180000);
	::Sleep(5000);

	//ö�ٽ���ģ����Ϣ<ģ��·��,ģ���ַ>
	std::map<std::string, DWORD64> process_module_list;
	if (this->BAC::BACTools::GetProcessModule(&process_module_list))
	{
		for (auto& pair : process_module_list)
		{
			PVOID image_handle = (PVOID)pair.second;

			DWORD64 text_address = (DWORD64)this->BACTools::GetPeSectiontAddress(image_handle, ".text");
			DWORD text_size = this->BACTools::GetPeSectionSize(image_handle, ".text");

			//��CRC32�б��в���Ԫ��
			this->_crc32_list[pair.first].emplace(text_address, text_size);
		}
	}
	else
	{
		//���߷�������ȡ����ģ���б�ʧ�ܣ�����һЩ��������
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
	
	//��ʼ��ʱ��CRC32ֵ�б�<�ڴ���ʼ��ַ,CRC32ֵ>
	static std::map<DWORD64, unsigned int> crc32_value_list;

	//��ʼ������ģ���б�
	static bool first_call = false;
	if (!first_call)
	{
		this->InitMemoryCRC32List();

		//����CRC32�������ʼֵ
		for (auto& pair : this->_crc32_list)
			for (auto& p : pair.second)
				crc32_value_list.insert(std::make_pair(p.first, this->CRC32((PVOID)p.first, p.second)));

		first_call = true;
	}

	//����CRC32ֵ�����
	for (auto& module_path : this->_crc32_list)
	{
		//printf("��ǰ����ģ�飺%s,", pair.first.c_str());
		for (auto& module_begin_address : module_path.second)
		{
			unsigned int crc32_value = this->CRC32((PVOID)module_begin_address.first, module_begin_address.second);
			//printf("��ַ��%p,CRC32ֵ��%p\n", tpair.first, crc32_value);
			if (crc32_value != crc32_value_list[module_begin_address.first])
			{
				//��⵽��ǰģ�鱻�޸ģ�����dump���ͱ����ļ�һ���ϴ������������ݻ���
				printf("checked current module been changed��%s\n", module_path.first.c_str());
				client->SendAbnormalGameData("checked current module been changed: " + module_path.first);
			}
		}
	}

	baclog->FunctionLog(__FUNCTION__, "Leave");
#if NDEBUG
	VMProtectEnd();
#endif
}

