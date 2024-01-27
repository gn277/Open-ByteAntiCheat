#include "../BAC-Helper.h"


#if _WIN64
DWORD64 BACHelper::FOAtoRVA(char* file_buffer, DWORD64 foa_address)
#else
DWORD BACHelper::FOAtoRVA(char* file_buffer, DWORD foa_address)
#endif
{
    PIMAGE_DOS_HEADER p_dos_header = NULL;
    PIMAGE_NT_HEADERS p_nt_header = NULL;
    PIMAGE_FILE_HEADER p_file_header = NULL;
    PIMAGE_SECTION_HEADER p_section_header = NULL;
    PIMAGE_OPTIONAL_HEADER p_option_header = NULL;
    DWORD number_of_section = NULL;
    //DWORD dwSizeofImage = NULL;
    //DWORD dwSizeofHeaders = NULL;
    DWORD size_of_p_option_header = NULL;
    DWORD virtual_address = NULL;
    DWORD size_of_raw_data = NULL;
    DWORD poip_nt_headerer_raw_data = NULL;

#if _WIN64
    DWORD64 image_base = NULL;
    DWORD64 rva_address = NULL;
#else
    DWORD image_base = NULL;
    DWORD rva_address = NULL;
#endif


    p_dos_header = (PIMAGE_DOS_HEADER)file_buffer;
    p_nt_header = (PIMAGE_NT_HEADERS)(file_buffer + p_dos_header->e_lfanew);
    p_file_header = (PIMAGE_FILE_HEADER)(&(p_nt_header->FileHeader));
    p_option_header = (PIMAGE_OPTIONAL_HEADER)(&(p_nt_header->OptionalHeader));
    image_base = p_option_header->ImageBase;
    number_of_section = p_file_header->NumberOfSections;
    //dwSizeofImage = p_option_header->SizeOfImage;
    //dwSizeofHeaders = p_option_header->SizeOfHeaders;
    size_of_p_option_header = p_file_header->SizeOfOptionalHeader;

    p_section_header = (PIMAGE_SECTION_HEADER)((char*)p_option_header + size_of_p_option_header);
    for (unsigned int i = 0; i < number_of_section; i++)
    {
        //prip_nt_headerf("%x", ((PIMAGE_SECTION_HEADER)((char*)p_section_header + 40))->VirtualAddress);
        if (foa_address >= p_section_header->PointerToRawData && foa_address < ((PIMAGE_SECTION_HEADER)((char*)p_section_header + 40))->PointerToRawData)
        {
            virtual_address = p_section_header->VirtualAddress;
            rva_address = foa_address - p_section_header->PointerToRawData + virtual_address;
            return rva_address;
        }

        p_section_header = (PIMAGE_SECTION_HEADER)((char*)p_section_header + 40);
    }
    if (rva_address == NULL)
    {
        p_section_header = (PIMAGE_SECTION_HEADER)((char*)p_section_header - 40);
        virtual_address = p_section_header->VirtualAddress;
        rva_address = foa_address - p_section_header->PointerToRawData + virtual_address;
        return rva_address;
    }

    return 0;
}

DWORD BACHelper::RVAtoFOA(DWORD roa_address, char* file_buffer)
{
    PIMAGE_DOS_HEADER p_dos_header = NULL;
    PIMAGE_NT_HEADERS p_nt_header = NULL;
    PIMAGE_FILE_HEADER p_file_header = NULL;
    PIMAGE_SECTION_HEADER p_section_header = NULL;
    PIMAGE_OPTIONAL_HEADER p_option_header = NULL;
    DWORD number_of_sections = NULL;
    //DWORD dwSizeofImage = NULL;
    //DWORD dwSizeofHeaders = NULL;
    DWORD size_of_option_header = NULL;
    DWORD virtual_address = NULL;
    DWORD size_of_raw_data = NULL;
    DWORD pointer_raw_data = NULL;
    //DWORD virtual_address = NULL;
#if _WIN64
    DWORD64 image_base = NULL;
#else
    DWORD image_base = NULL;
#endif
    DWORD foa_address = NULL;

    p_dos_header = (PIMAGE_DOS_HEADER)file_buffer;
    p_nt_header = (PIMAGE_NT_HEADERS)(file_buffer + p_dos_header->e_lfanew);
    p_file_header = (PIMAGE_FILE_HEADER)(&(p_nt_header->FileHeader));
    p_option_header = (PIMAGE_OPTIONAL_HEADER)(&(p_nt_header->OptionalHeader));
    image_base = p_option_header->ImageBase;
    number_of_sections = p_file_header->NumberOfSections;
    //dwSizeofImage = option_header->SizeOfImage;
    //dwSizeofHeaders = option_header->SizeOfHeaders;
    size_of_option_header = p_file_header->SizeOfOptionalHeader;

    p_section_header = (PIMAGE_SECTION_HEADER)((char*)p_option_header + size_of_option_header);
    //virtual_address = section_header->VirtualAddress;
    for (unsigned int i = 0; i < number_of_sections; i++)
    {
        //printf("%x", ((PIMAGE_SECTION_HEADER)((char*)section_header + 40))->VirtualAddress);
        if (roa_address >= p_section_header->VirtualAddress && roa_address < ((PIMAGE_SECTION_HEADER)((char*)p_section_header + 40))->VirtualAddress)
        {
            pointer_raw_data = p_section_header->PointerToRawData;
            foa_address = roa_address - p_section_header->VirtualAddress + pointer_raw_data;
            return foa_address;
        }

        p_section_header = (PIMAGE_SECTION_HEADER)((char*)p_section_header + 40);
    }
    if (foa_address == NULL)
    {
        p_section_header = (PIMAGE_SECTION_HEADER)((char*)p_section_header - 40);
        pointer_raw_data = p_section_header->PointerToRawData;
        foa_address = roa_address - p_section_header->VirtualAddress + pointer_raw_data;
        return foa_address;
    }

    return 0;
}

DWORD* BACHelper::AddSection(char* file_buffer, DWORD section_size, DWORD virtual_size)
{
    PIMAGE_DOS_HEADER p_dos_header = nullptr;
    PIMAGE_NT_HEADERS p_nt_header = nullptr;
    PIMAGE_FILE_HEADER p_file_header = nullptr;
    PIMAGE_SECTION_HEADER p_section_header = nullptr;
    PIMAGE_OPTIONAL_HEADER p_option_header = nullptr;

    p_dos_header = (PIMAGE_DOS_HEADER)file_buffer;
    p_nt_header = (PIMAGE_NT_HEADERS)(file_buffer + p_dos_header->e_lfanew);
    p_file_header = (PIMAGE_FILE_HEADER)(&(p_nt_header->FileHeader));
    p_option_header = (PIMAGE_OPTIONAL_HEADER)(&(p_nt_header->OptionalHeader));
    p_section_header = PIMAGE_SECTION_HEADER((char*)p_option_header + p_file_header->SizeOfOptionalHeader);

    //添加大小为sectionsize的节
    p_option_header->SizeOfImage = p_option_header->SizeOfImage + section_size;

    //计算后续的内存空间是否足够添加节表
    DWORD number_of_section = p_file_header->NumberOfSections;
    DWORD lfanew = p_dos_header->e_lfanew;
    DWORD size_of_header = p_option_header->SizeOfHeaders;
    DWORD size_now_header = lfanew + sizeof(*p_nt_header) + number_of_section * sizeof(*p_section_header);

    //判断后续空间是否足够添加节表，并移动头部信息
    if (size_of_header - size_now_header < 2 * sizeof(p_section_header))
    {
        memcpy(file_buffer + 0x40, file_buffer + p_dos_header->e_lfanew, sizeof(*p_nt_header) + number_of_section * sizeof(*p_section_header));

        p_dos_header->e_lfanew = 0x40;
        p_nt_header = (PIMAGE_NT_HEADERS)(file_buffer + p_dos_header->e_lfanew);
        p_file_header = (PIMAGE_FILE_HEADER)(&(p_nt_header->FileHeader));
        p_option_header = (PIMAGE_OPTIONAL_HEADER)(&(p_nt_header->OptionalHeader));
        p_section_header = PIMAGE_SECTION_HEADER((char*)p_option_header + p_file_header->SizeOfOptionalHeader);

        DWORD number_of_section = p_file_header->NumberOfSections;
        DWORD lfanew = p_dos_header->e_lfanew;
        DWORD size_of_header = p_option_header->SizeOfHeaders;
        DWORD size_now_header = lfanew + sizeof(*p_nt_header) + number_of_section * sizeof(*p_section_header);
    }
    //添加两个节表  移动到最后一个节表处
    for (unsigned int i = 0; i < number_of_section - 1; i++)
        p_section_header = p_section_header + 1;

    //添加节表
    DWORD add_section_address = size_now_header;
    PIMAGE_SECTION_HEADER p_add_section = (PIMAGE_SECTION_HEADER)malloc(sizeof(IMAGE_SECTION_HEADER));
    if (p_add_section == NULL)
        return NULL;

    memset(p_add_section, 0, sizeof(*p_add_section));
    char new_name[] = ".newSec";
    memcpy(p_add_section, new_name, 8);

    //计算内存中对齐的大小
    DWORD t_virtual_size = NULL;
    if (p_section_header->SizeOfRawData < p_section_header->Misc.VirtualSize)
        t_virtual_size = ((p_section_header->Misc.VirtualSize / 0x1000) + 1) * 0x1000;
    else
        t_virtual_size = (p_section_header->SizeOfRawData / 0x1000 + 1) * 0x1000;

    //添加节表信息
    p_add_section->VirtualAddress = p_section_header->VirtualAddress + t_virtual_size;
    p_add_section->Misc.VirtualSize = virtual_size;
    p_add_section->SizeOfRawData = section_size;
    p_add_section->PointerToRawData = p_section_header->PointerToRawData + p_section_header->SizeOfRawData;
    p_add_section->Characteristics = IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_WRITE;

    //写到file_buffer中
    memcpy(file_buffer + add_section_address, p_add_section, 40);
    char secode_section[40] = { NULL };
    memcpy(file_buffer + add_section_address + 40, secode_section, 40);

    //将节的数量加1
    p_file_header->NumberOfSections = p_file_header->NumberOfSections + 1;

    //申请新的空间
    DWORD* add_section_space = (DWORD*)malloc(_msize(file_buffer) + section_size);
    if (add_section_space == NULL)
        return NULL;

    memset(add_section_space, 0, _msize(add_section_space));
    memcpy(add_section_space, file_buffer, _msize(file_buffer));
    return add_section_space;
}

LPVOID BACHelper::ReadFileToBuffer(const char* file_path)
{
    int file_size = 0;
    FILE* fp = nullptr;

    fopen_s(&fp, file_path, "rb");
    if (fp == nullptr)
    {
        printf("file open error!\n");
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    LPVOID file_buffer = (LPVOID)malloc(file_size);
    if (file_buffer == nullptr)
        return NULL;

    memset(file_buffer, 0, file_size);

    fread(file_buffer, sizeof(char), file_size, fp);
    fclose(fp);
    return file_buffer;
}

bool BACHelper::ImportTableInject(const char* target_exe_path, const char* new_exe_path, const char* dll_name, const char* import_function_name)
{
#if NDEBUG
    VMProtectBegin("BACHelper::ImportTableInject");
#endif
    printf("target exe path:%s\n", target_exe_path);

    DWORD	Size = 0;
    LPVOID	pFileBuffer = NULL;
    pFileBuffer = this->ReadFileToBuffer(target_exe_path);
    if (pFileBuffer == NULL)
        return false;

    //新建一个节
    char* NewBuffer = (char*)this->AddSection((char*)pFileBuffer, 0x1000, 0x600);
    PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)NewBuffer;
    PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)(NewBuffer + dos->e_lfanew);

    PIMAGE_OPTIONAL_HEADER option = (PIMAGE_OPTIONAL_HEADER)(&(nt->OptionalHeader));

    PIMAGE_DATA_DIRECTORY ImportDIR = (PIMAGE_DATA_DIRECTORY)(&(option->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]));
    PIMAGE_IMPORT_DESCRIPTOR ImportAddressFA = (PIMAGE_IMPORT_DESCRIPTOR)(NewBuffer + this->RVAtoFOA((*(DWORD*)(ImportDIR)), NewBuffer));//IMPORT_DESCRIPTOR是目录相的第一项，大小为4字节
    //拷贝import_descriptor到新的节中
    //定位到最后一个节
    PIMAGE_FILE_HEADER file_header = (PIMAGE_FILE_HEADER)(&(nt->FileHeader));
    DWORD dwSizeofOptionHeader = file_header->SizeOfOptionalHeader;
    PIMAGE_SECTION_HEADER section_header = (PIMAGE_SECTION_HEADER)((char*)option + dwSizeofOptionHeader);
    DWORD AddNumSction = file_header->NumberOfSections;
    DWORD dwNumofSection = file_header->NumberOfSections;
    for (unsigned int i = 0; i < AddNumSction - 1; i++)
        section_header = (PIMAGE_SECTION_HEADER)((char*)section_header + 40);

    DWORD dwUnInitDataSize = section_header->Misc.VirtualSize;
    DWORD dwPointerRawData = section_header->PointerToRawData;
    DWORD dwVirtualAddress = section_header->VirtualAddress;
    DWORD dwCodeWirteAddress = dwPointerRawData;
    memset(NewBuffer + dwCodeWirteAddress, 0, 0x1000);
    DWORD i = 0;
    while (ImportAddressFA->OriginalFirstThunk && ImportAddressFA->FirstThunk)
    {
        memcpy(NewBuffer + dwCodeWirteAddress + i * sizeof(IMAGE_IMPORT_DESCRIPTOR), ImportAddressFA, sizeof(IMAGE_IMPORT_DESCRIPTOR));
        i++;
        ImportAddressFA = ImportAddressFA + 1;
    }
    PIMAGE_IMPORT_DESCRIPTOR newImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(NewBuffer + dwCodeWirteAddress + i * sizeof(IMAGE_IMPORT_DESCRIPTOR));
    PIMAGE_THUNK_DATA  newThunkDataOri = (PIMAGE_THUNK_DATA)(newImportDescriptor + 2);
    PIMAGE_THUNK_DATA  newThunkDatafirs = newThunkDataOri + 2;

    //计算dll_name地址
    DWORD* dll_nameAddress = (DWORD*)(newThunkDatafirs + 2);
    //拷贝
    memcpy(dll_nameAddress, dll_name, strlen(dll_name));
    //增加导入函数的名称
    //计算需要拷贝到的内存地址
    DWORD* FuncnameAddress = (DWORD*)((char*)dll_nameAddress + strlen(dll_name) + 1);
    //拷贝
    memcpy(FuncnameAddress, import_function_name, strlen(import_function_name));
    //修复各种地址

    newImportDescriptor->Name = (DWORD)this->FOAtoRVA(NewBuffer, (char*)dll_nameAddress - NewBuffer);
    newImportDescriptor->OriginalFirstThunk = (DWORD)this->FOAtoRVA(NewBuffer, (char*)newThunkDataOri - NewBuffer);
    newImportDescriptor->FirstThunk = (DWORD)this->FOAtoRVA(NewBuffer, (char*)newThunkDatafirs - NewBuffer);
    newThunkDataOri->u1.Function = (DWORD)this->FOAtoRVA(NewBuffer, (char*)FuncnameAddress - NewBuffer);
    newThunkDatafirs->u1.Function = (DWORD)this->FOAtoRVA(NewBuffer, (char*)FuncnameAddress - NewBuffer);
    //改变目录项地址
#if _WIN64
    *(DWORD64*)(ImportDIR) = this->FOAtoRVA(NewBuffer, dwCodeWirteAddress);
#else
    *(DWORD*)(ImportDIR) = this->FOAtoRVA(NewBuffer, dwCodeWirteAddress);
#endif

    FILE* fp = NULL;
    fopen_s(&fp, new_exe_path, "wb+");
    if (fp == NULL)
        return false;
    fwrite(NewBuffer, sizeof(char), _msize(NewBuffer), fp);
    fclose(fp);
    free(NewBuffer);
    NewBuffer = NULL;

    return true;
#if NDEBUG
    VMProtectEnd();
#endif
}


