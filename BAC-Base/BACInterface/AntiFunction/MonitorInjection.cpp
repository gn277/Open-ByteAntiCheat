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

	//在加载之前判断下该模块是否被加载过
	HMODULE hPreMod = GetModuleHandleW(szDllName);
	NTSTATUS ntStatus = pfnLdrLoadDll(PathToFile, Flags, ModuleFileName, ModuleHandle);
	DWORD dwLastError = GetLastError();

	//如果没有被加载过，在执行完pfnLdrLoadDll成功后检测是否做了隐藏自己的操作
	if (STATUS_SUCCESS == ntStatus && NULL == hPreMod)
	{
		//GetModuleHandleW其实是读取peb的消息,没必要再遍历一遍了
		HMODULE hNowMod = GetModuleHandleW(szDllName);
		if (NULL == hNowMod) 
			is_hidemodle = true;
	}

	//恢复错误码
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

	//获取相关函数地址
	pfnLdrLoadDll = (fpLdrLoadDll)::GetProcAddress(ntdll, "LdrLoadDll");

	//记录hook点
#if _WIN64
	std::map<std::string, DWORD64> hook_address = { {"LdrLoadDll", (DWORD64)pfnLdrLoadDll} };
#else
	std::map<std::string, DWORD> hook_address = { {"LdrLoadDll", (DWORD)pfnLdrLoadDll} };
#endif

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourAttach((PVOID*)&pfnLdrLoadDll, BACLdrLoadDll);

	DetourTransactionCommit();

	//记录Hook点并计算CRC32
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
		MessageBoxA(NULL, "检测到输入法注入", "提示", MB_OK);
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

	//记录hook点
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

	//记录Hook点并计算CRC32
	for (auto pair : hook_address)
		this->_hook_list[pair.first].emplace(pair.second, this->CRC32((void*)pair.second, 5));

	baclog->FunctionLog(__FUNCTION__, "Leave");
#if NDEBUG
	VMProtectEnd();
#endif
}


