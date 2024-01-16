////////////////////////////////////////////////////////////////
//						导出对外调用的接口
////////////////////////////////////////////////////////////////
#pragma once
#include <Windows.h>
#include <stdio.h>
#include <iostream>

#include "BACInterface/BAC.h"
#include "BACLog/BACLog.h"


extern "C" BAC* bac;
extern "C" BACLog* baclog;

extern "C" __declspec(dllexport) bool BACBaseInitialize();
extern "C" __declspec(dllexport) bool BACBaseUnInitialize();

