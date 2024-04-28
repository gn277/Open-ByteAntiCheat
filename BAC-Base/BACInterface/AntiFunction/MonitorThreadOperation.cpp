////////////////////////////////////////////////////////////////
//						在此监视线程状态
////////////////////////////////////////////////////////////////
#include "../BAC.h"

typedef NTSTATUS(WINAPI* pfNtSuspendThread)(_In_ HANDLE ThreadHandle, _Out_opt_ PULONG PreviousSuspendCount);
typedef NTSTATUS(WINAPI* pfNtResumeThread)(_In_ HANDLE ThreadHandle, _Out_opt_ PULONG PreviousSuspendCount);
typedef NTSTATUS(WINAPI* pfNtSetContextThread)(IN HANDLE ThreadHandle, IN PCONTEXT Context);
typedef NTSTATUS(WINAPI* pfNtGetContextThread)(IN HANDLE ThreadHandle, OUT PCONTEXT Context);

pfNtSuspendThread pfnNtSuspendThread = nullptr;
pfNtResumeThread pfnNtResumeThread = nullptr;
pfNtSetContextThread pfnNtSetContextThread = nullptr;
pfNtGetContextThread pfnNtGetContextThread = nullptr;


NTSTATUS BACNtSuspendThread(IN HANDLE thread_handle, OUT PULONG previous_suspend_count)
{
#if NDEBUG
	VMProtectBeginUltra("BACSuspendThread");
#endif

	NTSTATUS status = pfnNtSuspendThread(thread_handle, previous_suspend_count);

	return status;
#if NDEBUG
	VMProtectEnd();
#endif
}

NTSTATUS BACNtResumeThread(IN HANDLE thread_handle, OUT PULONG previous_suspend_count)
{
#if NDEBUG
	VMProtectBeginUltra("BACNtResumeThread");
#endif

	NTSTATUS status = pfnNtResumeThread(thread_handle, previous_suspend_count);

	return status;
#if NDEBUG
	VMProtectEnd();
#endif
}

NTSTATUS BACNtSetContextThread(IN HANDLE thread_handle, IN PCONTEXT context)
{
#if NDEBUG
	VMProtectBeginUltra("BACtSetContextThread");
#endif

	NTSTATUS status = pfnNtSetContextThread(thread_handle, context);

	return status;
#if NDEBUG
	VMProtectEnd();
#endif
}

NTSTATUS BACNtGetContextThread(IN HANDLE thread_handle, OUT PCONTEXT context)
{
#if NDEBUG
	VMProtectBeginUltra("BACNtGetContextThread");
#endif

	NTSTATUS status = pfnNtGetContextThread(thread_handle, context);

	return status;
#if NDEBUG
	VMProtectEnd();
#endif
}

void BAC::MonitorThreadOperation()
{
#if NDEBUG
	VMProtectBeginUltra("BAC::MonitorThreadOperation");
#endif
	baclog->FunctionLog(__FUNCTION__, "Enter");

	HMODULE ntdll = ::GetModuleHandleA("ntdll.dll");
	if (!ntdll)
		ntdll = ::LoadLibraryA("ntdll.dll");

	//获取相关函数地址
	pfnNtSuspendThread = (pfNtSuspendThread)::GetProcAddress(ntdll, "NtSuspendThread");
	pfnNtResumeThread = (pfNtResumeThread)::GetProcAddress(ntdll, "NtResumeThread");
	pfnNtSetContextThread = (pfNtSetContextThread)::GetProcAddress(ntdll, "NtSetContextThread");
	pfnNtGetContextThread = (pfNtGetContextThread)::GetProcAddress(ntdll, "NtGetContextThread");

#if _WIN64
	std::map<std::string, DWORD64> hook_address = {
		{ "NtSuspendThread", (DWORD64)pfnNtSuspendThread },
		{ "NtResumeThread", (DWORD64)pfnNtResumeThread },
		{ "NtSetContextThread", (DWORD64)pfnNtSetContextThread },
		{ "NtGetContextThread",(DWORD64)pfnNtGetContextThread } };
#else
	std::map<std::string, DWORD> hook_address = {
		{ "NtSuspendThread", (DWORD)pfnNtSuspendThread },
		{ "NtResumeThread", (DWORD)pfnNtResumeThread },
		{ "NtSetContextThread", (DWORD)pfnNtSetContextThread },
		{ "NtGetContextThread",(DWORD)pfnNtGetContextThread } };
#endif

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourAttach((PVOID*)&pfnNtSuspendThread, BACNtSuspendThread);
	DetourAttach((PVOID*)&pfnNtResumeThread, BACNtResumeThread);
	DetourAttach((PVOID*)&pfnNtSetContextThread, BACNtSetContextThread);
	DetourAttach((PVOID*)&pfnNtGetContextThread, BACNtGetContextThread);

	DetourTransactionCommit();

	//记录Hook点并计算CRC32
	for (auto pair : hook_address)
		this->_hook_list[pair.first].emplace(pair.second, this->CRC32((void*)pair.second, 5));

	baclog->FunctionLog(__FUNCTION__, "Leave");
#if NDEBUG
	VMProtectEnd();
#endif
}

