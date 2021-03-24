#pragma once
#include <Windows.h>
#include <iostream>
#include <unordered_map>
#include <algorithm>

#ifndef WZLDLL
#define WZL_EXPORT __declspec(dllexport)
#else
#define WZL_EXPORT
#endif // WZLDLL

#define WZLDLL  __stdcall