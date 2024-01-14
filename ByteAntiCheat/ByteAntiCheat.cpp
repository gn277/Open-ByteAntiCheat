#include <Windows.h>
#include <stdio.h>

#include "../BAC-Base/BAC-Base.h"
#if! NDEBUG
#pragma comment(lib,"../x64/Debug/BAC-Base64.lib")
#else
#pragma comment(lib,"../x64/Release/BAC-Base64.lib")
#endif


int main()
{
    //HMODULE hmodlue = LoadLibraryA("ByteAntiCheat\\BAC-Base.dll");
    //if (hmodlue)
    //{
    //
    //}
    //else
    //    printf("模块加载失败！\n");

    BACBaseInitialize();
    

    getchar();
    return 0;
}

