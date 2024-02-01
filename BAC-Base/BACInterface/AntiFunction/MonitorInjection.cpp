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
	VMProtectBeginUltra("BACLdrLoadDll");
#endif

	bool is_hidemodle = false;
	WCHAR szDllName[MAX_PATH];
	ZeroMemory(szDllName, sizeof(szDllName));
	memcpy(szDllName, ModuleFileName->Buffer, ModuleFileName->Length);

	printf("[LOG]:%s dll name:%S\n", __FUNCTION__, szDllName);

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
	VMProtectBeginUltra("BAC::MonitorLdrLoadDll");
#endif
	baclog->FunctionLog(__FUNCTION__, "Enter");

	HMODULE ntdll = ::GetModuleHandleA("ntdll.dll");
	if (!ntdll)
		ntdll = ::LoadLibraryA("ntdll.dll");

	//��ȡ��غ�����ַ
	pfnLdrLoadDll = (fpLdrLoadDll)::GetProcAddress(ntdll, "LdrLoadDll");

	//��¼hook��
#if _WIN64
	std::map<std::string, DWORD64> hook_address = { {"LdrLoadDll", (DWORD64)pfnLdrLoadDll} };
#else
	std::map<std::string, DWORD> hook_address = { {"LdrLoadDll", (DWORD)pfnLdrLoadDll} };
#endif

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourAttach((PVOID*)&pfnLdrLoadDll, BACLdrLoadDll);

	DetourTransactionCommit();

	//��¼Hook�㲢����CRC32
	for (auto pair : hook_address)
		this->_hook_list[pair.first].emplace(pair.second, this->CRC32((void*)pair.second, 5));

	baclog->FunctionLog(__FUNCTION__, "Leave");
#if NDEBUG
	VMProtectEnd();
#endif
}

void BAC::MonitorApc()
{
#if NDEBUG
	VMProtectBeginUltra("BAC::MonitorApc");
#endif
	baclog->FunctionLog(__FUNCTION__, "Enter");



	baclog->FunctionLog(__FUNCTION__, "Leave");
#if NDEBUG
	VMProtectEnd();
#endif
}

BOOL WINAPI BACImmGetHotKey(DWORD dwHotKeyID, LPUINT lpuModifiers, LPUINT lpuVKey, LPHKL lphKL)
{
#if NDEBUG
	VMProtectBeginUltra("BACImmGetHotKey");
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
	VMProtectBeginUltra("BACImmActivateLayout");
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
	VMProtectBeginUltra("BAC::MonitorImm");
#endif
	baclog->FunctionLog(__FUNCTION__, "Enter");

	HMODULE imm32 = LoadLibraryA("imm32.dll");
	pfnGetHotKey = (fpImmGetHotKey)GetProcAddress(imm32, "ImmGetHotKey");
	pfnImmActivateLayout = (fpImmActivateLayout)GetProcAddress(imm32, "ImmActivateLayout");

	//��¼hook��
#if _WIN64
	std::map<std::string, DWORD64> hook_address = {
		{ "ImmGetHotKey", (DWORD64)pfnGetHotKey },
		{ "ImmActivateLayout",(DWORD64)pfnImmActivateLayout } };
#else
	std::map<std::string, DWORD> hook_address = {
		{ "ImmGetHotKey", (DWORD)pfnGetHotKey },
		{ "ImmActivateLayout",(DWORD)pfnImmActivateLayout } };
#endif

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourAttach((PVOID*)&pfnGetHotKey, BACImmGetHotKey);
	DetourAttach((PVOID*)&pfnImmActivateLayout, BACImmActivateLayout);

	DetourTransactionCommit();

	//��¼Hook�㲢����CRC32
	for (auto pair : hook_address)
		this->_hook_list[pair.first].emplace(pair.second, this->CRC32((void*)pair.second, 5));

	baclog->FunctionLog(__FUNCTION__, "Leave");
#if NDEBUG
	VMProtectEnd();
#endif
}


