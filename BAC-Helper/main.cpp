#include <Windows.h>
#include <stdio.h>
#include <iostream>

#include "BAC-Helper.h"

BACHelper* bac_helper = nullptr;


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpcmdLine, _In_ int nCmdShow)
{
	//ʵ����BACHelper
	bac_helper = new BACHelper();




	//�ͷ�BACHelper
	if (bac_helper)
		delete bac_helper;

	return 0;
}

