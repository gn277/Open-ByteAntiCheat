#include <Windows.h>
#include <stdio.h>

#include "../BAC-Base/BAC-Base.h"
#if _DEBUG
//#pragma comment(lib,"../x64/Debug/ByteAntiCheat/BAC-Base.lib")
#pragma comment(lib,"../x64/Debug/BAC-Base.lib")
#else
#pragma comment(lib,"../x64/Release/ByteAntiCheat/BAC-Base.lib")
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

