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


class BAC : public BACKernel, public Tools
{
private:
	//Hook����������¼�ɹ�Hook��ַ����ѭ��Ч���Ƿ񱻻ָ�
#if _WIN64
	std::map<const std::string/*Api��*/, std::map<DWORD64/*��ַ*/, unsigned int/*Hook��CRC32ֵ*/>> _hook_list;
#else
	std::map<const std::string/*Api��*/, std::map<DWORD/*��ַ*/, unsigned int/*Hook��CRC32ֵ*/>> _hook_list;
#endif

	//CRC32List�ṹ��std::map<ģ��·��,<�ڴ���ʼ��ַ,�ڴ��С>>
	std::map<std::string, std::map<DWORD64, SIZE_T>> _crc32_list;

	//�ڴ�������ṹ��std::map<ģ����,<�ڴ��С,<�ڴ���ʼ��ַ,�ڴ������ַ>>>
	std::map<std::string, std::map<SIZE_T, std::map<DWORD64, DWORD64>>> _memory_whitelist;

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


	//����ӳ���ڴ�
	bool RemapImage(ULONG_PTR image_base);
	
	//����ѭ���¼�
	static void LoopEvent();
	void InitializeLoopEnvent();

	//�����ں���Ϣ���߳�
	static void RecvKernelMessage();
	//������Ϣ��ǲ����
	void KernelMessageDispathcer(void* message);

private:
	//��ȡ�ڴ�CRC32�б�
	bool InitMemoryCRC32List();

public:
	//���ڴ�������CRC32Ч��
	unsigned int CRC32(void* pdata, size_t data_len);
	//���Hook��״̬
	void CheckHookPointer();
	//������ģ���ڴ�
	void CheckMemoryCRC32();

public:
	BAC();
	~BAC();

};