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
#if NDEBUG
	VMProtectBegin("BAC::InitializeBACKernel");
#endif
#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Enter");
#endif

	//ʵ����BACKernel
	this->bac_kernel = new BACKernel();
	if (!this->bac_kernel)
	{
#if _DEBUG
		baclog->FileLog("bac kernel instantiate error");
		baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
		return false;
	}

	//��װ������Ҳ����ʹ��BACHelper.exe���а�װ��������Ϸ������ֱ��OpenDriverHandle�����ڸ��´˷�����
	if (!this->bac_kernel->InstiallDriver())
	{
#if _DEBUG
		baclog->FileLog("bac kernel install driver error");
		baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
		return false;
	}

	//���������
	if (!this->bac_kernel->OpenDriverHandle())
	{
#if _DEBUG
		baclog->FileLog("bac kernel open driver handle error");
		baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
		return false;
	}

#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
	return true;
#if NDEBUG
	VMProtectEnd();
#endif
}

bool BAC::UnInitializeBACKernel()
{
//#if NDEBUG
//	VMProtectBegin("BAC::UnInitializeBACKernel");
//#endif
#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Enter");
#endif

	//���ж�BACKernel�Ƿ�ʵ����
	if (this->bac_kernel)
	{
		//ж������
		if (!this->bac_kernel->UnInstallDriver())
		{
			delete this->bac_kernel;
#if _DEBUG
			baclog->FileLog("uninstall driver error");
			baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
			return false;
		}

		//�ͷ�BACKernel
		delete this->bac_kernel;
	}

#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
	return true;
//#if NDEBUG
//	VMProtectEnd();
//#endif
}

