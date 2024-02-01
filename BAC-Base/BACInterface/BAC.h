#pragma once
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include <utility>

#include "../Detours/include/detours.h"
#if _WIN64
#pragma comment(lib,"Detours/lib.X64/detours.lib")
#elif _WIN32
#pragma comment(lib,"Detours/lib.X86/detours.lib")
#endif

#include "SystemApi.h"
#include "BACKernelApi/BACKernel.h"
#include "../BACTools/BACTools.h"

using namespace std;


class BAC : public Tools
{
private:
	BACKernel* bac_kernel = nullptr;

	//Hook����������¼�ɹ�Hook��ַ����ѭ��Ч���Ƿ񱻻ָ�
#if _WIN64
	std::map<const std::string/*Api��*/, std::map<DWORD64/*��ַ*/, unsigned int/*Hook��CRC32ֵ*/>> _hook_list;
#else
	std::map<const std::string/*Api��*/, std::map<DWORD/*��ַ*/, unsigned int/*Hook��CRC32ֵ*/>> _hook_list;
#endif

private:
	void MakePePacked(HANDLE hProcess, PBYTE pImageBuff);
	bool JudgmentHookModule(PVOID hook_address);

public:
	//����Ӧ�ò��hook
	void HideHook();
	//���Ӵ��ڴ�����غ���
	void MonitorCreateWindow();
	//����LdrLoadDllע��
	void MonitorLdrLoadDll();
	//����APCע��
	void MonitorApc();
	//�������뷨ע��
	void MonitorImm();
	//�����ڴ��д����
	void MonitorMemoryOption();
	
	//����ѭ���¼�
	static void LoopEvent();

public:
	//���ڴ�������CRC32Ч��
	unsigned int CRC32(void* pdata, size_t data_len);
	//���Hook��״̬
	void CheckHookPointer();

public:
	//BAC�ں˽ӿڳ�ʼ��
	bool InitializeBACKernel();
	bool UnInitializeBACKernel();

public:
	BAC();
	~BAC();

};