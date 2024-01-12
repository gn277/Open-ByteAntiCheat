#include "BAC-Base.h"

BAC* bac = nullptr;


bool BACBaseInitialize()
{
	//实例化BAC对象
	bac = new BAC();

	//应用层隐藏hook
	bac->HideHook();
	//监视窗口创建的相关函数
	bac->MonitorCreateWindow();



	printf("BAC Initialize Over...\n");

	return true;
}

bool BACBaseUnInitialize()
{
	//释放BAC对象
	if (bac)
		delete bac;

	return true;
}

