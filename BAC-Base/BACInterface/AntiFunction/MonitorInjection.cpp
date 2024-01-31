#include "../BAC.h"
#include "../../BAC-Base.h"

typedef NTSTATUS(WINAPI* fpLdrLoadDll)(IN PWCHAR PathToFile OPTIONAL, IN PULONG Flags OPTIONAL,
	IN PUNICODE_STRING ModuleFileName, OUT PHANDLE ModuleHandle);

fpLdrLoadDll pfnLdrLoadDll = NULL;

typedef BOOL(__stdcall* fpImmGetHotKey)(DWORD dwHotKeyID, LPUINT lpuModifiers, LPUINT lpuVKey, LPHKL lphKL);
typedef int(__stdcall* fpImmActivateLayout)(LPARAM);

bool active_flag = false;
fpImmGetHotKey pfnGetHotKey = NULL;
fpImmActivateLayout pfnImmActivateLayout = NULL;


NTSTATUS WINAPI BACLdrLoadDll(IN PWCHAR PathToFile OPTIONAL, IN PULONG Flags OPTIONAL,
	IN PUNICODE_STRING ModuleFileName, OUT PHANDLE ModuleHandle)
{
#if NDEBUG
	VMProtectBegin("BACLdrLoadDll");
#endif

	bool is_hidemodle = false;
	WCHAR szDllName[MAX_PATH];
	ZeroMemory(szDllName, sizeof(szDllName));
	memcpy(szDllName, ModuleFileName->Buffer, ModuleFileName->Length);

	printf("[LOG]:LdrLoadDll:%S\n", szDllName);

	//�ڼ���֮ǰ�ж��¸�ģ���Ƿ񱻼��ع�
	HMODULE hPreMod = GetModuleHandleW(szDllName);
	NTSTATUS ntStatus = pfnLdrLoadDll(PathToFile, Flags, ModuleFileName, ModuleHandle);
	DWORD dwLastError = GetLastError();

	//���û�б����ع�����ִ����pfnLdrLoadDll�ɹ������Ƿ����������Լ��Ĳ���
	if (STATUS_SUCCESS == ntStatus && NULL == hPreMod)
	{
		//GetModuleHandleW��ʵ�Ƕ�ȡpeb����Ϣ,û��Ҫ�ٱ���һ����
		HMODULE hNowMod = GetModuleHandleW(szDllName);
		if (NULL == hNowMod) 
			is_hidemodle = true;
	}

	//�ָ�������
	SetLastError(dwLastError);
	return ntStatus;

#if NDEBUG
	VMProtectEnd();
#endif
}

void BAC::MonitorLdrLoadDll()
{
#if NDEBUG
	VMProtectBegin("BAC::MonitorLdrLoadDll");
#endif
#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Enter");
#endif

	HMODULE ntdll = ::GetModuleHandleA("ntdll.dll");
	if (!ntdll)
		ntdll = ::LoadLibraryA("ntdll.dll");

	//��ȡ��غ�����ַ
	pfnLdrLoadDll = (fpLdrLoadDll)::GetProcAddress(ntdll, "LdrLoadDll");

	//��¼hook��
#if _WIN64
	this->_hook_list.insert(std::make_pair("LdrLoadDll", (DWORD64)pfnLdrLoadDll));
#else
	this->_hook_list.insert(std::make_pair("LdrLoadDll", (DWORD)pfnLdrLoadDll));
#endif

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourAttach((PVOID*)&pfnLdrLoadDll, BACLdrLoadDll);

	DetourTransactionCommit();

#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
#if NDEBUG
	VMProtectEnd();
#endif
}

void BAC::MonitorApc()
{
#if NDEBUG
	VMProtectBegin("BAC::MonitorApc");
#endif
#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Enter");
#endif



#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
#if NDEBUG
	VMProtectEnd();
#endif
}

BOOL WINAPI BACImmGetHotKey(DWORD dwHotKeyID, LPUINT lpuModifiers, LPUINT lpuVKey, LPHKL lphKL)
{
#if NDEBUG
	VMProtectBegin("BACImmGetHotKey");
#endif

	active_flag = true;
	return pfnGetHotKey(dwHotKeyID, lpuModifiers, lpuVKey, lphKL);

#if NDEBUG
	VMProtectEnd();
#endif
}

int WINAPI BACImmActivateLayout(LPARAM pa)
{
#if NDEBUG
	VMProtectBegin("BACImmActivateLayout");
#endif

	if (active_flag)
		active_flag = true;
	else
	{
		MessageBoxA(NULL, "��⵽���뷨ע��", "��ʾ", MB_OK);
		ExitProcess(5);
	}
	return pfnImmActivateLayout(pa);

#if NDEBUG
	VMProtectEnd();
#endif
}

void BAC::MonitorImm()
{
#if NDEBUG
	VMProtectBegin("BAC::MonitorImm");
#endif
#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Enter");
#endif

	HMODULE imm32 = LoadLibraryA("imm32.dll");
	pfnGetHotKey = (fpImmGetHotKey)GetProcAddress(imm32, "ImmGetHotKey");
	pfnImmActivateLayout = (fpImmActivateLayout)GetProcAddress(imm32, "ImmActivateLayout");

	//��¼hook��
#if _WIN64
	this->_hook_list.insert({
		{ "ImmGetHotKey", (DWORD64)pfnGetHotKey },
		{ "ImmActivateLayout",(DWORD64)pfnImmActivateLayout } });
#else
	this->_hook_list.insert({
		{ "ImmGetHotKey", (DWORD)pfnGetHotKey },
		{ "ImmActivateLayout",(DWORD)pfnImmActivateLayout } });
#endif

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach((PVOID*)&pfnGetHotKey, BACImmGetHotKey);
	DetourAttach((PVOID*)&pfnImmActivateLayout, BACImmActivateLayout);
	DetourTransactionCommit();

#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
#if NDEBUG
	VMProtectEnd();
#endif
}


