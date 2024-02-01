////////////////////////////////////////////////////////////////
//						�ڴ˼��Hook��
////////////////////////////////////////////////////////////////
#include "../BAC.h"
#include "../../BAC-Base.h"


bool BAC::JudgmentHookModule(PVOID hook_address)
{
#if NDEBUG
	VMProtectBegin("BAC::JudgmentHookModule");
#endif

	DWORD64 bac_module_end = (DWORD64)this->Tools::GetModuleEndAddress(self_module);

	//����jmp��ת���ĵ�ַ
	DWORD64 target_address = ((DWORD64)hook_address + 5) + this->Tools::ReadInt((PVOID)((DWORD64)hook_address + 1));

	//�ж��Ƿ�������
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

	return true;
#if NDEBUG
	VMProtectEnd();
#endif
}

void BAC::CheckHookPointer()
{
#if NDEBUG
	VMProtectBegin("BAC::CheckHookPointer");
#endif

	for (auto pair : this->_hook_list)
	{
		for (auto tpair : pair.second)
		{
			//Ч��CRC32
			if (tpair.second != this->CRC32((void*)tpair.first, 5))
				std::cout << "[LOG]:" << __FUNCTION__ << " " << pair.first << " CRC32 value is error" << std::endl;

			//Ч��Hook��jmp����λ�Ƿ���ת��BACģ����
			if (!this->JudgmentHookModule((void*)tpair.first))
				std::cout << "[LOG]:" << __FUNCTION__ << " hook to address not bac module!" << std::endl;
		}
	}

#if NDEBUG
	VMProtectEnd();
#endif
}
