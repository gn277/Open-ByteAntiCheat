#pragma once
#include <Windows.h>
#include <stdio.h>
#include <iostream>

//#include "BACInterface/BAC.h"
#include "BACInterface/BAC.h"


extern "C" BAC* bac;

extern "C" __declspec(dllexport) bool BACBaseInitialize();
extern "C" __declspec(dllexport) bool BACBaseUnInitialize();

