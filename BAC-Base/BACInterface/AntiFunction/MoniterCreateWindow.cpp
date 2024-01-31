#include "../BAC.h"
#include "../../BAC-Base.h"

//创建窗口函数
typedef WORD(WINAPI* fpRegisterClassExA)(WNDCLASSEXA* lpWndCls);
typedef WORD(WINAPI* fpRegisterClassA)(WNDCLASSA* lpWndClass);

typedef WORD(WINAPI* fpRegisterClassExW)(WNDCLASSEXW* lpWndCls);
typedef WORD(WINAPI* fpRegisterClassW)(WNDCLASSW* lpWndClass);

typedef HWND(WINAPI* fpCreateWindowExW)(
	__in DWORD dwExStyle, __in_opt LPCWSTR lpClassName, __in_opt LPCWSTR lpWindowName,
	__in DWORD dwStyle, __in int X, __in int Y, __in int nWidth, __in int nHeight,
	__in_opt HWND hWndParent, __in_opt HMENU hMenu, __in_opt HINSTANCE hInstance, __in_opt LPVOID lpParam);

typedef HWND(WINAPI* fpCreateWindowExA)(
	__in DWORD dwExStyle, __in_opt LPCSTR lpClassName, __in_opt LPCSTR lpWindowName, __in DWORD dwStyle,
	__in int X, __in int Y, __in int nWidth, __in int nHeight, __in_opt HWND hWndParent, __in_opt HMENU hMenu,
	__in_opt HINSTANCE hInstance, __in_opt LPVOID lpParam);

typedef BOOL(WINAPI* fpSetWindowTextW)(_In_ HWND hWnd, _In_opt_ LPCWSTR lpString);
typedef BOOL(WINAPI* fpSetWindowTextA)(_In_ HWND hWnd, _In_opt_ LPCSTR lpString);


fpRegisterClassExA pfnRegisterClassExA = NULL;
fpRegisterClassA pfnRegisterClassA = NULL;
fpRegisterClassExW pfnRegisterClassExW = NULL;
fpRegisterClassW pfnRegisterClassW = NULL;
fpCreateWindowExW pfnCreateWindowExW = NULL;
fpCreateWindowExA pfnCreateWindowExA = NULL;
fpSetWindowTextW pfnSetWindowTextW = NULL;
fpSetWindowTextA pfnSetWindowTextA = NULL;


DWORD WINAPI BACRegisterClassExA(WNDCLASSEXA* lpWndCls)
{
#if NDEBUG
	VMProtectBegin("BACRegisterClassExA");
#endif

	if (lpWndCls->lpszClassName)
	{
		CHAR szNewClassName[MAX_PATH];
		DWORD dwOld;
		OutputDebugStringA(lpWndCls->lpszClassName);
		VirtualProtect((LPVOID)lpWndCls->lpszClassName, strlen(lpWndCls->lpszClassName) + 1, PAGE_EXECUTE_READWRITE, &dwOld);
		wsprintfA(szNewClassName, "%d_%d", GetTickCount(), GetCurrentProcessId());
		RtlCopyMemory((PVOID)lpWndCls->lpszClassName, (PVOID)szNewClassName, strlen(lpWndCls->lpszClassName));
	}

	return pfnRegisterClassExA(lpWndCls);

#if NDEBUG
	VMProtectEnd();
#endif
}

DWORD WINAPI BACRegisterClassA(WNDCLASSA* lpWndClass)
{
#if NDEBUG
	VMProtectBegin("BACRegisterClassA");
#endif

	if (lpWndClass->lpszClassName)
	{
		CHAR szNewClassName[MAX_PATH];
		DWORD dwOld;
		OutputDebugStringA(lpWndClass->lpszClassName);
		VirtualProtect((LPVOID)lpWndClass->lpszClassName, strlen(lpWndClass->lpszClassName) + 1, PAGE_EXECUTE_READWRITE, &dwOld);
		wsprintfA(szNewClassName, "%d_%d", GetTickCount(), GetCurrentProcessId());
		RtlCopyMemory((PVOID)lpWndClass->lpszClassName, (PVOID)szNewClassName, strlen(lpWndClass->lpszClassName));
	}

	return pfnRegisterClassA(lpWndClass);

#if NDEBUG
	VMProtectEnd();
#endif
}

DWORD WINAPI BACRegisterClassExW(WNDCLASSEXW* lpWndCls)
{
#if NDEBUG
	VMProtectBegin("BACRegisterClassExW");
#endif

	if (lpWndCls->lpszClassName)
	{
		WCHAR szNewClassName[MAX_PATH];
		DWORD dwOld;
		OutputDebugStringW(lpWndCls->lpszClassName);
		VirtualProtect((LPVOID)lpWndCls->lpszClassName, wcslen(lpWndCls->lpszClassName) + 1, PAGE_EXECUTE_READWRITE, &dwOld);
		wsprintfW(szNewClassName, L"%d_%d", GetTickCount(), GetCurrentProcessId());
		RtlCopyMemory((PVOID)lpWndCls->lpszClassName, (PVOID)szNewClassName, wcslen(lpWndCls->lpszClassName) * 2);
	}

	return pfnRegisterClassExW(lpWndCls);

#if NDEBUG
	VMProtectEnd();
#endif
}

DWORD WINAPI BACRegisterClassW(WNDCLASSW* lpWndClass)
{
#if NDEBUG
	VMProtectBegin("BACRegisterClassW");
#endif

	if (lpWndClass->lpszClassName)
	{
		WCHAR szNewClassName[MAX_PATH];
		DWORD dwOld;
		OutputDebugStringW(lpWndClass->lpszClassName);
		VirtualProtect((LPVOID)lpWndClass->lpszClassName, wcslen(lpWndClass->lpszClassName) + 1, PAGE_EXECUTE_READWRITE, &dwOld);
		wsprintfW(szNewClassName, L"%d_%d", GetTickCount(), GetCurrentProcessId());
		RtlCopyMemory((PVOID)lpWndClass->lpszClassName, (PVOID)szNewClassName, wcslen(lpWndClass->lpszClassName) * 2);
	}

	return pfnRegisterClassW(lpWndClass);

#if NDEBUG
	VMProtectEnd();
#endif
}

HWND WINAPI BACCreateWindowExW(
	__in DWORD dwExStyle, __in_opt LPCWSTR lpClassName, __in_opt LPCWSTR lpWindowName, __in DWORD dwStyle,
	__in int X, __in int Y, __in int nWidth, __in int nHeight, __in_opt HWND hWndParent, __in_opt HMENU hMenu,
	__in_opt HINSTANCE hInstance, __in_opt LPVOID lpParam)
{
#if NDEBUG
	VMProtectBegin("BACCreateWindowExW");
#endif

	if (lpWindowName)
	{
		WCHAR szNewWndName[MAX_PATH];
		wsprintfW(szNewWndName, L"%d_%ws", GetCurrentProcessId(), lpWindowName);
		lpWindowName = szNewWndName;
	}

	return  pfnCreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

#if NDEBUG
	VMProtectEnd();
#endif
}

HWND WINAPI BACCreateWindowExA(
	__in DWORD dwExStyle, __in_opt LPCSTR lpClassName, __in_opt LPCSTR lpWindowName, __in DWORD dwStyle,
	__in int X, __in int Y, __in int nWidth, __in int nHeight, __in_opt HWND hWndParent, __in_opt HMENU hMenu,
	__in_opt HINSTANCE hInstance, __in_opt LPVOID lpParam)
{
#if NDEBUG
	VMProtectBegin("BACCreateWindowExA");
#endif

	if (lpWindowName)
	{
		CHAR szNewWndName[MAX_PATH];
		wsprintfA(szNewWndName, "%d_%s", GetCurrentProcessId(), lpWindowName);
		lpWindowName = szNewWndName;
	}

	return  pfnCreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

#if NDEBUG
	VMProtectEnd();
#endif
}

BOOL WINAPI BACSetWindowTextW(_In_ HWND hWnd, _In_opt_  LPCTSTR lpString)
{
#if NDEBUG
	VMProtectBegin("BACSetWindowTextW");
#endif

	if (lpString)
	{
		WCHAR buff[MAX_PATH];
		wsprintf(buff, L"%d_%s", GetCurrentProcessId(), lpString);
	}

	return pfnSetWindowTextW(hWnd, lpString);

#if NDEBUG
	VMProtectEnd();
#endif
}

BOOL WINAPI BACSetWindowTextA(_In_ HWND hWnd, _In_opt_ LPCSTR lpString)
{
#if NDEBUG
	VMProtectBegin("BACSetWindowTextA");
#endif
#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Enter");
#endif

	if (lpString)
	{
		char buff[MAX_PATH];
		wsprintfA(buff, "%d_%s", GetCurrentProcessId(), lpString);
	}

	return pfnSetWindowTextA(hWnd, lpString);

#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
#if NDEBUG
	VMProtectEnd();
#endif
}

void BAC::MonitorCreateWindow()
{
#if NDEBUG
	VMProtectBegin("BAC::MonitorCreateWindow");
#endif
#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Enter");
#endif

	HMODULE user32 = ::GetModuleHandleA("user32.dll");
	if (!user32)
		user32 = ::LoadLibraryA("user32.dll");

	//获取相关函数地址
	pfnRegisterClassExA = (fpRegisterClassExA)::GetProcAddress(user32, "RegisterClassExA");
	pfnRegisterClassA = (fpRegisterClassA)::GetProcAddress(user32, "RegisterClassA");
	pfnRegisterClassExW = (fpRegisterClassExW)::GetProcAddress(user32, "RegisterClassExW");
	pfnRegisterClassW = (fpRegisterClassW)::GetProcAddress(user32, "RegisterClassW");
	pfnCreateWindowExW = (fpCreateWindowExW)::GetProcAddress(user32, "CreateWindowExW");
	pfnCreateWindowExA = (fpCreateWindowExA)::GetProcAddress(user32, "CreateWindowExA");
	pfnSetWindowTextW = (fpSetWindowTextW)::GetProcAddress(user32, "SetWindowTextW");
	pfnSetWindowTextA = (fpSetWindowTextA)::GetProcAddress(user32, "SetWindowTextA");

	//记录hook点
#if _WIN64
	this->_hook_list.insert({
		{ "RegisterClassExA", (DWORD64)pfnRegisterClassExA },
		{ "RegisterClassA",(DWORD64)pfnRegisterClassA },
		{ "RegisterClassExW",(DWORD64)pfnRegisterClassExW },
		{ "RegisterClassW",(DWORD64)pfnRegisterClassW },
		{ "CreateWindowExW",(DWORD64)pfnCreateWindowExW} ,
		{ "CreateWindowExA",(DWORD64)pfnCreateWindowExA },
		{ "SetWindowTextW",(DWORD64)pfnSetWindowTextW },
		{ "SetWindowTextA",(DWORD64)pfnSetWindowTextA } });
#else
	this->_hook_list.insert({
		{ "RegisterClassExA", (DWORD)pfnRegisterClassExA },
		{ "RegisterClassA",(DWORD)pfnRegisterClassA },
		{ "RegisterClassExW",(DWORD)pfnRegisterClassExW },
		{ "RegisterClassW",(DWORD)pfnRegisterClassW },
		{ "CreateWindowExW",(DWORD)pfnCreateWindowExW} ,
		{ "CreateWindowExA",(DWORD)pfnCreateWindowExA },
		{ "SetWindowTextW",(DWORD)pfnSetWindowTextW },
		{ "SetWindowTextA",(DWORD)pfnSetWindowTextA } });
#endif

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourAttach((PVOID*)&pfnRegisterClassExA, BACRegisterClassExA);
	DetourAttach((PVOID*)&pfnRegisterClassA, BACRegisterClassA);
	DetourAttach((PVOID*)&pfnRegisterClassExW, BACRegisterClassExW);
	DetourAttach((PVOID*)&pfnRegisterClassW, BACRegisterClassW);
	DetourAttach((PVOID*)&pfnCreateWindowExW, BACCreateWindowExW);
	DetourAttach((PVOID*)&pfnCreateWindowExA, BACCreateWindowExA);
	DetourAttach((PVOID*)&pfnSetWindowTextW, BACSetWindowTextW);
	DetourAttach((PVOID*)&pfnSetWindowTextA, BACSetWindowTextA);

	DetourTransactionCommit();

#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
#if NDEBUG
	VMProtectEnd();
#endif
}

