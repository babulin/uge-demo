#pragma once
#include <Windows.h>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include "zlibwapi.h"
//#include <d3d9.h>
//#include <d3dx9.h>

#ifndef WZLDLL
#define WZL_EXPORT __declspec(dllexport)
#else
#define WZL_EXPORT
#endif // WZLDLL

#define WZLDLL  __stdcall