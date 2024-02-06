#include "BACTools.h"


Tools::Tools()
{
}

Tools::~Tools()
{
}

PVOID64 Tools::GetModuleEndAddress(HMODULE module_handle)
{
	PVOID64 return_address = 0;
	MODULEINFO moduleinfo = { 0 };

	GetModuleInformation(::GetCurrentProcess(), module_handle, &moduleinfo, sizeof(MODULEINFO));

	return return_address = (PVOID64)((__int64)moduleinfo.lpBaseOfDll + moduleinfo.SizeOfImage);
}

ULONG64 Tools::ReadULong64(PVOID address)
{
	if (IsBadReadPtr(address, 8))
		return 0;

	return *(ULONG64*)address;
}

LONG64 Tools::ReadLong64(PVOID address)
{
	if (IsBadReadPtr(address, 8))
		return 0;

	return *(LONG64*)address;
}

ULONG Tools::ReadULong(PVOID address)
{
	if (IsBadReadPtr(address, 4))
		return 0;

	return *(ULONG*)address;
}

UINT Tools::ReadUInt(PVOID address)
{
	if (IsBadReadPtr(address, 4))
		return 0;

	return *(UINT*)address;
}

INT Tools::ReadInt(PVOID address)
{
	if (IsBadReadPtr(address, 4))
		return 0;

	return *(INT*)address;
}

BYTE Tools::ReadByte(PVOID address)
{
	if (IsBadReadPtr(address, sizeof(BYTE)))
		return 0;

	return *(BYTE*)address;
}

PVOID Tools::GetPeSectiontAddress(PVOID image, const char* section_name)
{
	PIMAGE_NT_HEADERS p_nt_header = (PIMAGE_NT_HEADERS)((ULONG_PTR)image + ((PIMAGE_DOS_HEADER)image)->e_lfanew);
	if (!p_nt_header)
		return 0;

	PIMAGE_OPTIONAL_HEADER p_option_header = (PIMAGE_OPTIONAL_HEADER)(&(p_nt_header->OptionalHeader));
	PIMAGE_FILE_HEADER p_file_header = (PIMAGE_FILE_HEADER)(&(p_nt_header->FileHeader));
	PIMAGE_SECTION_HEADER p_section_header = (PIMAGE_SECTION_HEADER)((char*)p_option_header + p_file_header->SizeOfOptionalHeader);

	//±éÀú½Ú
	for (WORD i = 0; i < p_nt_header->FileHeader.NumberOfSections; ++i)
	{
		DWORD64 section_base = (p_nt_header->OptionalHeader.ImageBase + p_section_header[i].VirtualAddress);

		if (_stricmp(section_name, (const char*)p_section_header[i].Name) == 0)
		{
			//printf("    %-8.8s    0x%IX - 0x%IX,  0x%08X\n",
			//	p_section_header[i].Name,
			//	section_base,
			//	section_base + p_section_header[i].Misc.VirtualSize,
			//	p_section_header[i].Misc.VirtualSize);

			return (PVOID)section_base;
		}
	}

}

