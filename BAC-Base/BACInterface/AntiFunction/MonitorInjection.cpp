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


NTSTATUS WINAPI MyLdrLoadDll(IN PWCHAR PathToFile OPTIONAL, IN PULONG Flags OPTIONAL,
	IN PUNICODE_STRING ModuleFileName, OUT PHANDLE ModuleHandle)
{
	bool is_hidemodle = false;
	WCHAR szDllName[MAX_PATH];
	ZeroMemory(szDllName, sizeof(szDllName));
	memcpy(szDllName, ModuleFileName->Buffer, ModuleFileName->Length);

	printf("[LOG]:LdrLoadDll:%S\n", szDllName);

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
}

void BAC::MonitorLdrLoadDll()
{
#if _DEBUG
	baclog->OutPutCommandLine(__FUNCTION__, "Initialize");
#endif

	HMODULE ntdll = ::GetModuleHandleA("ntdll.dll");
	if (!ntdll)
		ntdll = ::LoadLibraryA("ntdll.dll");

	//获取相关函数地址
	pfnLdrLoadDll = (fpLdrLoadDll)::GetProcAddress(ntdll, "LdrLoadDll");

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourAttach((PVOID*)&pfnLdrLoadDll, MyLdrLoadDll);

	DetourTransactionCommit();

#if _DEBUG
	baclog->OutPutCommandLine(__FUNCTION__, "Leave");
#endif
}

void BAC::MonitorApc()
{

}

BOOL WINAPI MyImmGetHotKey(DWORD dwHotKeyID, LPUINT lpuModifiers, LPUINT lpuVKey, LPHKL lphKL)
{
	active_flag = true;
	return pfnGetHotKey(dwHotKeyID, lpuModifiers, lpuVKey, lphKL);
}

int WINAPI MyImmActivateLayout(LPARAM pa)
{
	if (active_flag)
		active_flag = true;
	else
	{
		MessageBoxA(NULL, "检测到输入法注入", "提示", MB_OK);
		ExitProcess(5);
	}
	return pfnImmActivateLayout(pa);
}

void BAC::MonitorImm()
{
#if _DEBUG
	baclog->OutPutCommandLine(__FUNCTION__, "Initialize");
#endif

	HMODULE imm32 = LoadLibraryA("imm32.dll");
	pfnGetHotKey = (fpImmGetHotKey)GetProcAddress(imm32, "ImmGetHotKey");
	pfnImmActivateLayout = (fpImmActivateLayout)GetProcAddress(imm32, "ImmActivateLayout");

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach((PVOID*)&pfnGetHotKey, MyImmGetHotKey);
	DetourAttach((PVOID*)&pfnImmActivateLayout, MyImmActivateLayout);
	DetourTransactionCommit();

#if _DEBUG
	baclog->OutPutCommandLine(__FUNCTION__, "Leave");
#endif
}


