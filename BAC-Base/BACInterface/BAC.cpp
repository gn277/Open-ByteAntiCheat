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

	//实例化BACKernel
	this->bac_kernel = new BACKernel();
	if (!this->bac_kernel)
	{
#if _DEBUG
		baclog->FileLog("bac kernel instantiate error");
		baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
		return false;
	}

	//安装驱动（也可以使用BACHelper.exe进行安装驱动后游戏主进程直接OpenDriverHandle，后期更新此方案）
	if (!this->bac_kernel->InstiallDriver())
	{
#if _DEBUG
		baclog->FileLog("bac kernel install driver error");
		baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
		return false;
	}

	//打开驱动句柄
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

	//先判断BACKernel是否实例化
	if (this->bac_kernel)
	{
		//卸载驱动
		if (!this->bac_kernel->UnInstallDriver())
		{
			delete this->bac_kernel;
#if _DEBUG
			baclog->FileLog("uninstall driver error");
			baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
			return false;
		}

		//释放BACKernel
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

