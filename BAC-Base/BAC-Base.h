////////////////////////////////////////////////////////////////
//						����������õĽӿ�
////////////////////////////////////////////////////////////////
#pragma once
#include <stdio.h>
#include <iostream>
#include <Windows.h>


extern "C" __declspec(dllexport) bool BACBaseInitialize(HMODULE module_handle);
extern "C" __declspec(dllexport) bool BACBaseUnInitialize();

