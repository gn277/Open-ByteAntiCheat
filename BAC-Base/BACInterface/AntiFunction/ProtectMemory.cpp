#include "../BAC.h"

typedef NTSTATUS(NTAPI* fpNtProtectVirtualMemory)(IN HANDLE ProcessHandle, IN OUT PVOID* BaseAddress, IN OUT PULONG ProtectSize, IN ULONG NewProtect, OUT PULONG OldProtect);
typedef BOOL(WINAPI* fpIsBadReadPtr)(_In_opt_ CONST VOID* lp, _In_ UINT_PTR ucb);
typedef BOOL(WINAPI* fpIsBadWritePtr)(_In_opt_ LPVOID lp, _In_ UINT_PTR ucb);

fpNtProtectVirtualMemory pfnNtProtectVirtualMemory = NULL;
fpIsBadReadPtr pfnIsBadReadPtr = NULL;
fpIsBadWritePtr pfnIsBadWritePtr = NULL;


NTSTATUS NTAPI MyNtProtectVirtualMemory(IN HANDLE ProcessHandle, IN OUT PVOID* BaseAddress, IN OUT PULONG ProtectSize, IN ULONG NewProtect, OUT PULONG OldProtect)
{
	printf("MyNtProtectVirtualMemory ...\n");

	return pfnNtProtectVirtualMemory(ProcessHandle, BaseAddress, ProtectSize, NewProtect, OldProtect);
}

BOOL WINAPI MyIsBadReadPtr(_In_opt_ CONST VOID* lp, _In_ UINT_PTR ucb)
{
	printf("MyIsBadReadPtr ...\n");

	return pfnIsBadReadPtr(lp, ucb);
}

BOOL WINAPI MyIsBadWritePtr(_In_opt_ LPVOID lp, _In_ UINT_PTR ucb)
{
	printf("MyIsBadWritePtr ...\n");

	return pfnIsBadWritePtr(lp, ucb);
}

void BAC::MonitorMemoryOption()
{
	HMODULE ntdll = ::GetModuleHandleA("ntdll.dll");
	HMODULE kernel32 = ::GetModuleHandleA("KERNEL32.dll");
	pfnNtProtectVirtualMemory = (fpNtProtectVirtualMemory)::GetProcAddress(ntdll, "NtProtectVirtualMemory");
	pfnIsBadReadPtr = (fpIsBadReadPtr)::GetProcAddress(kernel32, "IsBadReadPtr");
	pfnIsBadWritePtr = (fpIsBadWritePtr)::GetProcAddress(kernel32, "IsBadWritePtr");

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourAttach((PVOID*)&pfnNtProtectVirtualMemory, MyNtProtectVirtualMemory);
	DetourAttach((PVOID*)&pfnIsBadReadPtr, MyIsBadReadPtr);
	DetourAttach((PVOID*)&pfnIsBadWritePtr, MyIsBadWritePtr);

	DetourTransactionCommit();
}

