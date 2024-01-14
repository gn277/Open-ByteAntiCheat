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
    
    ////测试调用函数
    //WNDCLASSEX window_class = { sizeof(WNDCLASSEX), CS_CLASSDC, NULL, 0L, 0L, GetModuleHandle(NULL), LoadIcon(GetModuleHandle(NULL), (LPCWSTR)NULL), NULL, NULL, NULL, L"Test-window-class-name", NULL};
    //::RegisterClassExW(&window_class);


    getchar();
    return 0;
}

