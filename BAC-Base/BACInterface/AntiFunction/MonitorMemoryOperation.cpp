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
	VMProtectBeginUltra("BACNtProtectVirtualMemory");
#endif

	printf("%s stack info:\n", __FUNCTION__);
	bac->Tools::GetStackInfoList();

	//printf("%s-> new attribute:%p,old attribute:%p\n", __FUNCTION__, NewProtect, *(ULONG*)OldProtect);

	return pfnNtProtectVirtualMemory(ProcessHandle, BaseAddress, ProtectSize, NewProtect, OldProtect);

#if NDEBUG
	VMProtectEnd();
#endif
}

BOOL WINAPI BACIsBadReadPtr(_In_opt_ CONST VOID* lp, _In_ UINT_PTR ucb)
{
#if NDEBUG
	VMProtectBeginUltra("BACIsBadReadPtr");
#endif

	//printf("%s stack info:\n", __FUNCTION__);
	//bac->Tools::GetStackInfoList();

	return pfnIsBadReadPtr(lp, ucb);

#if NDEBUG
	VMProtectEnd();
#endif
}

BOOL WINAPI BACIsBadWritePtr(_In_opt_ LPVOID lp, _In_ UINT_PTR ucb)
{
#if NDEBUG
	VMProtectBeginUltra("BACIsBadWritePtr");
#endif

	//printf("%s-> lp:%p,ucb:%d\n", __FUNCTION__, lp, ucb);

	return pfnIsBadWritePtr(lp, ucb);

#if NDEBUG
	VMProtectEnd();
#endif
}

void BAC::MonitorMemoryOperation()
{
#if NDEBUG
	VMProtectBeginUltra("MonitorMemoryOperation");
#endif
	baclog->FunctionLog(__FUNCTION__, "Enter");

	HMODULE ntdll = ::GetModuleHandleA("ntdll.dll");
	HMODULE kernel32 = ::GetModuleHandleA("KERNEL32.dll");
	pfnNtProtectVirtualMemory = (fpNtProtectVirtualMemory)::GetProcAddress(ntdll, "NtProtectVirtualMemory");
	pfnIsBadReadPtr = (fpIsBadReadPtr)::GetProcAddress(kernel32, "IsBadReadPtr");
	pfnIsBadWritePtr = (fpIsBadWritePtr)::GetProcAddress(kernel32, "IsBadWritePtr");

	//记录hook点
#if _WIN64
	std::map<std::string, DWORD64> hook_address = {
		{ "NtProtectVirtualMemory", (DWORD64)pfnNtProtectVirtualMemory },
		{ "IsBadReadPtr",(DWORD64)pfnIsBadReadPtr },
		{ "IsBadWritePtr",(DWORD64)pfnIsBadWritePtr } };
#else
	std::map<std::string, DWORD> hook_address = {
		{ "NtProtectVirtualMemory", (DWORD)pfnNtProtectVirtualMemory },
		{ "IsBadReadPtr",(DWORD)pfnIsBadReadPtr },
		{ "IsBadWritePtr",(DWORD)pfnIsBadWritePtr } };
#endif

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourAttach((PVOID*)&pfnNtProtectVirtualMemory, BACNtProtectVirtualMemory);
	DetourAttach((PVOID*)&pfnIsBadReadPtr, BACIsBadReadPtr);
	DetourAttach((PVOID*)&pfnIsBadWritePtr, BACIsBadWritePtr);

	DetourTransactionCommit();

	//记录Hook点并计算CRC32
	for (auto pair : hook_address)
		this->_hook_list[pair.first].emplace(pair.second, this->CRC32((void*)pair.second, 5));

	baclog->FunctionLog(__FUNCTION__, "Leave");
#if NDEBUG
	VMProtectEnd();
#endif
}

