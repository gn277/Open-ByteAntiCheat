#include "../BAC.h"
#include "../../BAC-Base.h"


void BAC::MakePePacked(HANDLE hProcess, PBYTE pImageBuff)
{
#if NDEBUG
	VMProtectBegin("BAC::MakePePacked");
#endif

	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pImageBuff;
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)(pImageBuff + pDosHeader->e_lfanew);

	DWORD dwOld = 0;
	VirtualProtectEx(hProcess, pNtHeader, sizeof(PIMAGE_NT_HEADERS), PAGE_EXECUTE_READWRITE, &dwOld);

	pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = 1;
	pNtHeader->OptionalHeader.AddressOfEntryPoint = 0;

	VirtualProtectEx(hProcess, pNtHeader, sizeof(PIMAGE_NT_HEADERS), dwOld, &dwOld);

#if NDEBUG
	VMProtectEnd();
#endif
}

void BAC::HideHook()
{
//#if NDEBUG
//	VMProtectBegin("BAC::HideHook");
//#endif
#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Enter");
#endif

	DWORD dwProcessId = GetCurrentProcessId();
	NTSTATUS Status;
	HANDLE hProcess = NULL;
	PfnZwQueryInformationProcess fnZwQueryInformationProcess;
	PROCESS_BASIC_INFORMATION ProcessInfo;
	PPEB    pPeb;
	fnZwQueryInformationProcess = (PfnZwQueryInformationProcess)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "ZwQueryInformationProcess");
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, dwProcessId);


	Status = fnZwQueryInformationProcess(hProcess, 0, &ProcessInfo, sizeof(ProcessInfo), NULL);
	if (NT_SUCCESS(Status))
	{
		pPeb = (PPEB)ProcessInfo.PebBaseAddress;

		//for (PLIST_ENTRY pListEntry = pPeb->Ldr->InLoadOrderModuleList.Flink; pListEntry != &pPeb->Ldr->InLoadOrderModuleList; pListEntry = pListEntry->Flink)
		//{
		//	PLDR_DATA_TABLE_ENTRY pEntry = CONTAINING_RECORD(pListEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
		//	this->MakePePacked(hProcess, (PBYTE)pEntry->DllBase);
		//}
	}
	CloseHandle(hProcess);

#if _DEBUG
	baclog->FunctionLog(__FUNCTION__, "Leave");
#endif
//#if NDEBUG
//	VMProtectEnd();
//#endif
}