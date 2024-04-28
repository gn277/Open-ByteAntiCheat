////////////////////////////////////////////////////////////////
//						导出对外调用的接口
////////////////////////////////////////////////////////////////
#pragma once
#include <Windows.h>
#include <stdio.h>
#include <iostream>

#include "ProtectSDK/VMProtectSDK/VMProtectSDK.h"
#if _WIN64
#pragma comment(lib,"ProtectSDK/VMProtectSDK/VMProtectSDK64.lib")
#elif _WIN32
#pragma comment(lib,"ProtectSDK/VMProtectSDK/VMProtectSDK32.lib")
#else
#error "Unsupported platforms"
#endif

#include "BACInterface/BAC.h"
#include "BACLog/BACLog.h"


extern "C" std::shared_ptr<BAC> bac;
extern "C" std::shared_ptr<BACLog> baclog;
extern "C" HMODULE self_module;

extern "C" __declspec(dllexport) bool BACBaseInitialize();
extern "C" __declspec(dllexport) bool BACBaseUnInitialize();

