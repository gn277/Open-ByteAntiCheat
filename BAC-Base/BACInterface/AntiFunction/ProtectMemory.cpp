////////////////////////////////////////////////////////////////
//						监视内存读写函数
////////////////////////////////////////////////////////////////
#include "../BAC.h"
#include "../../BAC-Base.h"

typedef NTSTATUS(NTAPI* fpNtProtectVirtualMemory)(IN HANDLE ProcessHandle, IN OUT PVOID* BaseAddress, IN OUT PULONG ProtectSize, IN ULONG NewProtect, OUT PULONG OldProtect);
typedef BOOL(WINAPI* fpIsBadReadPtr)(_In_opt_ CONST VOID* lp, _In_ UINT_PTR ucb);
typedef BOOL(WINAPI* fpIsBadWritePtr)(_In_opt_ LPVOID lp, _In_ UINT_PTR ucb);

fpNtProtectVirtualMemory pfnNtProtectVirtualMemory = NULL;
fpIsBadReadPtr pfnIsBadReadPtr = NULL;
fpIsBadWritePtr pfnIsBadWritePtr = NULL;


NTSTATUS NTAPI BACNtProtectVirtualMemory(IN HANDLE ProcessHandle, IN OUT PVOID* BaseAddress, IN OUT PULONG ProtectSize, IN ULONG NewProtect, OUT PULONG OldProtect)
{
#if NDEBUG
	VMProtectBegin("BACNtProtectVirtualMemory");
#endif

	printf("MyNtProtectVirtualMemory ...\n");

	return pfnNtProtectVirtualMemory(ProcessHandle, BaseAddress, ProtectSize, NewProtect, OldProtect);

#if NDEBUG
	VMProtectEnd();
#endif
}

BOOL WINAPI BACIsBadReadPtr(_In_opt_ CONST VOID* lp, _In_ UINT_PTR ucb)
{
#if NDEBUG
	VMProtectBegin("BACIsBadReadPtr");
#endif

	printf("MyIsBadReadPtr ...\n");

	return pfnIsBadReadPtr(lp, ucb);

#if NDEBUG
	VMProtectEnd();
#endif
}

BOOL WINAPI BACIsBadWritePtr(_In_opt_ LPVOID lp, _In_ UINT_PTR ucb)
{
#if NDEBUG
	VMProtectBegin("BACIsBadWritePtr");
#endif

	printf("MyIsBadWritePtr ...\n");

	return pfnIsBadWritePtr(lp, ucb);

#if NDEBUG
	VMProtectEnd();
#endif
}

void BAC::MonitorMemoryOption()
{
#if NDEBUG
	VMProtectBegin("MonitorMemoryOption");
#endif
#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Enter");
#endif

	HMODULE ntdll = ::GetModuleHandleA("ntdll.dll");
	HMODULE kernel32 = ::GetModuleHandleA("KERNEL32.dll");
	pfnNtProtectVirtualMemory = (fpNtProtectVirtualMemory)::GetProcAddress(ntdll, "NtProtectVirtualMemory");
	pfnIsBadReadPtr = (fpIsBadReadPtr)::GetProcAddress(kernel32, "IsBadReadPtr");
	pfnIsBadWritePtr = (fpIsBadWritePtr)::GetProcAddress(kernel32, "IsBadWritePtr");

	//记录hook点
#if _WIN64
	this->_hook_list.insert({
		{ "NtProtectVirtualMemory", (DWORD64)pfnNtProtectVirtualMemory },
		{ "IsBadReadPtr",(DWORD64)pfnIsBadReadPtr },
		{ "IsBadWritePtr",(DWORD64)pfnIsBadWritePtr } });
#else
	this->_hook_list.insert({
		{ "NtProtectVirtualMemory", (DWORD)pfnNtProtectVirtualMemory },
		{ "IsBadReadPtr",(DWORD)pfnIsBadReadPtr },
		{ "IsBadWritePtr",(DWORD)pfnIsBadWritePtr } });
#endif

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourAttach((PVOID*)&pfnNtProtectVirtualMemory, BACNtProtectVirtualMemory);
	DetourAttach((PVOID*)&pfnIsBadReadPtr, BACIsBadReadPtr);
	DetourAttach((PVOID*)&pfnIsBadWritePtr, BACIsBadWritePtr);

	DetourTransactionCommit();

#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
#if NDEBUG
	VMProtectEnd();
#endif
}

