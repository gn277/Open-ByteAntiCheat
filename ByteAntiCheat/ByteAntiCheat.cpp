#include <Windows.h>
#include <stdio.h>

typedef bool(__stdcall* pfBACBaseInitialize)(void);
typedef bool(__stdcall* pfBACBaseUnInitialize)(void);


int main()
{
    //加载dll
    auto bac_module = ::LoadLibraryA("ByteAntiCheat/BAC-Base64.dll");
    if (!bac_module)
    {
        printf("BAC 模块加载失败！\n");
        return 0;
    }

    pfBACBaseInitialize bac_initialize = (pfBACBaseInitialize)::GetProcAddress(bac_module, "BACBaseInitialize");
    pfBACBaseUnInitialize bac_uninitialize = (pfBACBaseInitialize)::GetProcAddress(bac_module, "BACBaseUnInitialize");
    if (!bac_initialize || !bac_uninitialize)
    {
        printf("BAC 函数获取失败！\n");
        return 0;
    }

    //初始化加载BAC
    if (!bac_initialize())
        printf("BAC加载失败！\n");
    
    //测试窗口注册
    WNDCLASSEX window_class = { sizeof(WNDCLASSEX), CS_CLASSDC, NULL, 0L, 0L, GetModuleHandle(NULL), LoadIcon(GetModuleHandle(NULL), (LPCWSTR)NULL), NULL, NULL, NULL, L"Test-window-class-name", NULL};
    ::RegisterClassExW(&window_class);
    //测试读内存
    IsBadReadPtr((VOID*)::GetModuleHandleA(NULL), 4);

    //卸载BAC
    printf("回车卸载BAC\n");
    getchar();
    if (!bac_uninitialize())
        printf("BAC卸载失败！\n");

    printf("BAC卸载成功，回车退出...\n");
    getchar();
    return 0;
}

