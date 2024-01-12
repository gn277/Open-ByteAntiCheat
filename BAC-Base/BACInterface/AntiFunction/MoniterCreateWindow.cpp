#include "../BAC.h"
#include <Windows.h>

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
fpSetWindowTextW pfnSetWindowsTextW = NULL;
fpSetWindowTextA pfnSetWindowsTextA = NULL;


DWORD
WINAPI
MyRegisterClassExA(WNDCLASSEXA* lpWndCls)
{
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
}

DWORD WINAPI MyRegisterClassA(WNDCLASSA* lpWndClass)
{
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
}

DWORD WINAPI MyRegisterClassExW(WNDCLASSEXW* lpWndCls)
{
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
}

DWORD WINAPI MyRegisterClassW(WNDCLASSW* lpWndClass)
{
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
}

HWND WINAPI MyCreateWindowExW(
	__in DWORD dwExStyle, __in_opt LPCWSTR lpClassName, __in_opt LPCWSTR lpWindowName, __in DWORD dwStyle,
	__in int X, __in int Y, __in int nWidth, __in int nHeight, __in_opt HWND hWndParent, __in_opt HMENU hMenu,
	__in_opt HINSTANCE hInstance, __in_opt LPVOID lpParam)
{
	if (lpWindowName)
	{
		WCHAR szNewWndName[MAX_PATH];
		wsprintfW(szNewWndName, L"%d_%ws", GetCurrentProcessId(), lpWindowName);
		lpWindowName = szNewWndName;
	}
	return  pfnCreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

HWND WINAPI MyCreateWindowExA(
	__in DWORD dwExStyle, __in_opt LPCSTR lpClassName, __in_opt LPCSTR lpWindowName, __in DWORD dwStyle,
	__in int X, __in int Y, __in int nWidth, __in int nHeight, __in_opt HWND hWndParent, __in_opt HMENU hMenu,
	__in_opt HINSTANCE hInstance, __in_opt LPVOID lpParam)
{
	if (lpWindowName)
	{
		CHAR szNewWndName[MAX_PATH];
		wsprintfA(szNewWndName, "%d_%s", GetCurrentProcessId(), lpWindowName);
		lpWindowName = szNewWndName;
	}

	return  pfnCreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}


BOOL WINAPI MySetWindowTextW(_In_ HWND hWnd, _In_opt_  LPCTSTR lpString)
{
	if (lpString)
	{
		WCHAR buff[MAX_PATH];
		wsprintf(buff, L"%d_%s", GetCurrentProcessId(), lpString);
	}
	return pfnSetWindowsTextW(hWnd, lpString);
}

BOOL WINAPI MySetWindowTextA(_In_ HWND hWnd, _In_opt_ LPCSTR lpString)
{
	if (lpString)
	{
		char buff[MAX_PATH];
		wsprintfA(buff, "%d_%s", GetCurrentProcessId(), lpString);
	}

	return pfnSetWindowsTextA(hWnd, lpString);
}

void BAC::MonitorCreateWindow()
{
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
	pfnSetWindowsTextW = (fpSetWindowTextW)::GetProcAddress(user32, "SetWindowTextW");
	pfnSetWindowsTextA = (fpSetWindowTextA)::GetProcAddress(user32, "SetWindowTextA");

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourAttach((PVOID*)&pfnRegisterClassExA, MyRegisterClassExA);
	DetourAttach((PVOID*)&pfnRegisterClassA, MyRegisterClassA);
	DetourAttach((PVOID*)&pfnRegisterClassExW, MyRegisterClassExW);
	DetourAttach((PVOID*)&pfnRegisterClassW, MyRegisterClassW);
	DetourAttach((PVOID*)&pfnCreateWindowExW, MyCreateWindowExW);
	DetourAttach((PVOID*)&pfnCreateWindowExA, MyCreateWindowExA);


	DetourTransactionCommit();
}

