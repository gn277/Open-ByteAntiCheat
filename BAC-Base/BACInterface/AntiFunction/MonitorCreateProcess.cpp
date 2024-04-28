#include "../BAC.h"
#include "../SystemApi.h"

#define STATUS_INFO_LENGTH_MISMATCH      ((NTSTATUS)0xC0000004L)


BOOL IsProcessType(HANDLE handle)
{
	NTSTATUS status;
	BOOL ret = TRUE;
	POBJECT_TYPE_INFORMATION obj_type_info = NULL;

	obj_type_info = (POBJECT_TYPE_INFORMATION)VirtualAlloc(NULL, sizeof(OBJECT_TYPE_INFORMATION) + 0x1000, MEM_COMMIT, PAGE_READWRITE);
	status = ZwQueryObject(handle, ObjectTypeInformation, obj_type_info, sizeof(OBJECT_TYPE_INFORMATION) + 0x1000, NULL);
	if (NT_SUCCESS(status))
	{
		if (_wcsicmp(obj_type_info->TypeName.Buffer, L"Process") != 0)
			ret = FALSE;
	}

	VirtualFree(obj_type_info, 0, MEM_RELEASE);
	return ret;
}

HANDLE DumpHandle(HANDLE process_id, HANDLE handle_value)
{
	HANDLE handle = 0;
	OBJECT_ATTRIBUTES object_attributes;
	NTSTATUS status;
	HANDLE process_handle;
	CLIENT_ID cid = { 0 };
	cid.UniqueProcess = process_id;
	InitializeObjectAttributes(&object_attributes, NULL, 0, NULL, NULL);
	status = ZwOpenProcess(&process_handle, PROCESS_ALL_ACCESS, &object_attributes, &cid);

	if (NT_SUCCESS(status))
	{
		TCHAR file_name[MAX_PATH] = { 0 };
		GetProcessImageFileNameW(process_handle, file_name, _countof(file_name));
		PathStripPathW(file_name);


		if (!_tcscmp(file_name, _T("csrss.exe")) ||
			!_tcscmp(file_name, _T("lsass.exe")) ||
			!_tcscmp(file_name, _T("svchost.exe")) ||
			!_tcscmp(file_name, _T("explorer.exe")))
			return NULL;

		HANDLE local_handle;
		status = ZwDuplicateObject(
			process_handle,
			handle_value,
			GetCurrentProcess(),
			&local_handle,
			0L,
			0L,
			DUPLICATE_SAME_ACCESS | DUPLICATE_SAME_ATTRIBUTES
		);//进行对象复制
		if (NT_SUCCESS(status))
		{
			//这里准备查询！
			handle = local_handle;
		}
		_tprintf(TEXT("找到一个\n"));
		CloseHandle(process_handle);
	}

	return handle;
}

VOID KillHandle(HANDLE process_id, HANDLE handle_value)
{
	OBJECT_ATTRIBUTES obj_attributes;
	NTSTATUS status;
	HANDLE process_handle;
	CLIENT_ID cid = { 0 };

	if (::GetCurrentProcessId() == (DWORD)process_id)
		return;

	cid.UniqueProcess = process_id;
	InitializeObjectAttributes(&obj_attributes, NULL, 0, NULL, NULL);
	status = ZwOpenProcess(&process_handle, PROCESS_ALL_ACCESS, &obj_attributes, &cid);
	if (NT_SUCCESS(status))
	{
		HANDLE local_handle;

		status = ZwDuplicateObject(
			process_handle,
			process_id,
			GetCurrentProcess(),
			&local_handle,
			0L,
			0L,
			DUPLICATE_CLOSE_SOURCE
		);//进行对象Kill
		if (NT_SUCCESS(status))
		{
			CloseHandle(local_handle);
		}
		CloseHandle(process_handle);
	}
}

void BAC::ClearOtherProcessHandle()
{
#if NDEBUG
	VMProtectBeginUltra("BAC::ClearOtherProcessHandle");
#endif

	while (1)
	{
		ULONG nsize = 0;

		NTSTATUS ns = ZwQuerySystemInformation(SystemHandleInformation, NULL, 0, &nsize);
		if (ns == STATUS_INFO_LENGTH_MISMATCH)
		{
			//////////////////////////////////////////////////////////////////////////
			//首先通过NULL参数获取Buffer的大小,然后申请Buffer
			//////////////////////////////////////////////////////////////////////////
		Loop:
			PVOID p_buffer = NULL;
			p_buffer = VirtualAlloc(NULL, nsize * 2, MEM_COMMIT, PAGE_READWRITE);//如果在内核形态这里是使用Pool或者其他内核内存分配形态！！！
			if (p_buffer)
			{
				RtlZeroMemory(p_buffer, nsize * 2);
				ns = ZwQuerySystemInformation(SystemHandleInformation, p_buffer, nsize * 2, NULL);
				if (NT_SUCCESS(ns))
				{
					//////////////////////////////////////////////////////////////////////////
					//第二次调用获取Handle成功了，这里开始写遍历了！！！
					//////////////////////////////////////////////////////////////////////////
					ULONG nindex = 0;
					PSYSTEM_HANDLE_INFORMATION p_system_handle_info = NULL;
					p_system_handle_info = (PSYSTEM_HANDLE_INFORMATION)p_buffer;
					for (nindex = 0; nindex < p_system_handle_info->NumberOfHandles; nindex++)
					{
						if (p_system_handle_info->Handles[nindex].UniqueProcessId != GetCurrentProcessId())
						{
							//不是本进程的东西，则先打开目标进程！
							HANDLE local_handle = DumpHandle((HANDLE)p_system_handle_info->Handles[nindex].UniqueProcessId, (HANDLE)p_system_handle_info->Handles[nindex].HandleValue);
							if (local_handle)
							{
								if (IsProcessType(local_handle))
								{
									TCHAR szFileName[MAX_PATH] = { 0 };
									GetProcessImageFileName(local_handle, szFileName, MAX_PATH);
									if (GetProcessId(local_handle) == GetCurrentProcessId())
										KillHandle((HANDLE)p_system_handle_info->Handles[nindex].UniqueProcessId, (HANDLE)p_system_handle_info->Handles[nindex].HandleValue);
								}
								//关闭句柄
								CloseHandle(local_handle);
							}
						}
						else
						{
							if (p_system_handle_info->Handles[nindex].UniqueProcessId != GetCurrentProcessId())
							{
								if (IsProcessType((HANDLE)p_system_handle_info->Handles[nindex].HandleValue))
									CloseHandle((HANDLE)p_system_handle_info->Handles[nindex].HandleValue);
							}

						}
					}
				}

				VirtualFree(p_buffer, 0, MEM_RELEASE);//释放内存
				if (ns == STATUS_INFO_LENGTH_MISMATCH)//如果是内存不够，则扩大10倍继续申请！！
				{
					nsize = nsize * 10;
					goto Loop;
				}
			}
		}
		Sleep(1000);
	}

#if NDEBUG
	VMProtectEnd();
#endif
}

