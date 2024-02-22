#pragma once

extern "C"
{
	NTSTATUS ZwQuerySystemInformation(
		IN ULONG SystemInformationClass,
		IN PVOID SystemInformation,
		IN ULONG SystemInformationLength,
		OUT PULONG ReturnLength);

	PVOID NTAPI RtlImageDirectoryEntryToData(IN PVOID  BaseAddress,
		IN BOOLEAN        ImageLoaded,
		IN ULONG  Directory,
		OUT PULONG        Size);

	NTSYSAPI NTSTATUS NTAPI ZwQueryInformationProcess(
		IN HANDLE ProcessHandle,
		IN PROCESSINFOCLASS ProcessInformationClass,
		OUT PVOID ProcessInformation,
		IN ULONG ProcessInformationLength,
		IN PULONG ReturnLength);

	PVOID PsGetProcessDebugPort(IN PEPROCESS Process);
	NTKERNELAPI PVOID PsGetProcessPeb(IN PEPROCESS Process);
	NTKERNELAPI HANDLE PsGetProcessId(IN PEPROCESS Process);
	PCHAR PsGetProcessImageFileName(IN PEPROCESS p_process);

}


