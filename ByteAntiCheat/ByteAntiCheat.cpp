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
    //初始化加载BAC
    if (!BACBaseInitialize())
        printf("BAC加载失败！\n");
    
    //测试窗口注册
    WNDCLASSEX window_class = { sizeof(WNDCLASSEX), CS_CLASSDC, NULL, 0L, 0L, GetModuleHandle(NULL), LoadIcon(GetModuleHandle(NULL), (LPCWSTR)NULL), NULL, NULL, NULL, L"Test-window-class-name", NULL};
    ::RegisterClassExW(&window_class);
    //测试读内存
    IsBadReadPtr((VOID*)::GetModuleHandleA(NULL), 4);

    //卸载BAC
    printf("回车卸载BAC\n");
    getchar();
    if (!BACBaseUnInitialize())
        printf("BAC卸载失败！\n");

    printf("BAC卸载成功，回车退出...\n");
    getchar();
    return 0;
}

