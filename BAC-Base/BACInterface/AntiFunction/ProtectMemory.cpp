#include "../BAC.h"

typedef NTSTATUS(NTAPI* fpNtProtectVirtualMemory)(IN HANDLE ProcessHandle, IN OUT PVOID* BaseAddress, IN OUT PULONG ProtectSize, IN ULONG NewProtect, OUT PULONG OldProtect);

fpNtProtectVirtualMemory pfnNtProtectVirtualMemory = NULL;


NTSTATUS NTAPI MyNtProtectVirtualMemory(IN HANDLE ProcessHandle, IN OUT PVOID* BaseAddress, IN OUT PULONG ProtectSize, IN ULONG NewProtect, OUT PULONG OldProtect)
{

	printf("MyNtProtectVirtualMemory ...\n");

	return pfnNtProtectVirtualMemory(ProcessHandle, BaseAddress, ProtectSize, NewProtect, OldProtect);
}

void BAC::MonitorMemoryOption()
{
	HMODULE ntdll = ::GetModuleHandleA("ntdll.dll");
	pfnNtProtectVirtualMemory = (fpNtProtectVirtualMemory)::GetProcAddress(ntdll, "NtProtectVirtualMemory");


	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourAttach((PVOID*)&pfnNtProtectVirtualMemory, MyNtProtectVirtualMemory);

	DetourTransactionCommit();
}

