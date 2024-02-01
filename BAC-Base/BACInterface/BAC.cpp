#include "BAC.h"
#include "../BAC-Base.h"


BAC::BAC()
{
}

BAC::~BAC()
{
}

bool BAC::InitializeBACKernel()
{
	baclog->FunctionLog(__FUNCTION__, "Enter");

	//ʵ����BACKernel
	this->bac_kernel = new BACKernel();
	if (!this->bac_kernel)
	{
		baclog->FileLog("bac kernel instantiate error");
		baclog->FunctionLog(__FUNCTION__, "Leave");
		return false;
	}

#if NDEBUG
	VMProtectBeginUltra("BAC::InitializeBACKernel");
#endif
	//��װ������Ҳ����ʹ��BACHelper.exe���а�װ��������Ϸ������ֱ��OpenDriverHandle�����ڸ��´˷�����
	if (!this->bac_kernel->InstiallDriver())
	{
		baclog->FileLog("bac kernel install driver error");
		baclog->FunctionLog(__FUNCTION__, "Leave");
		return false;
	}

	//���������
	if (!this->bac_kernel->OpenDriverHandle())
	{
		baclog->FileLog("bac kernel open driver handle error");
		baclog->FunctionLog(__FUNCTION__, "Leave");
		return false;
	}
#if NDEBUG
	VMProtectEnd();
#endif

	baclog->FunctionLog(__FUNCTION__, "Leave");
	return true;
}

bool BAC::UnInitializeBACKernel()
{
#if NDEBUG
	VMProtectBeginUltra("BAC::UnInitializeBACKernel");
#endif

	baclog->FunctionLog(__FUNCTION__, "Enter");

	//���ж�BACKernel�Ƿ�ʵ����
	if (this->bac_kernel)
	{
		//ж������
		if (!this->bac_kernel->UnInstallDriver())
		{
			delete this->bac_kernel;
			baclog->FileLog("uninstall driver error");
			baclog->FunctionLog(__FUNCTION__, "Leave");

			return false;
		}

		//�ͷ�BACKernel
		delete this->bac_kernel;
	}

	baclog->FunctionLog(__FUNCTION__, "Leave");
	return true;

#if NDEBUG
	VMProtectEnd();
#endif
}

