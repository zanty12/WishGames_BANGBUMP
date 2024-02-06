#pragma once
#include <string>
#ifdef _DEBUG
#define PARAM_PATH std::wstring(L"data/property/")
#else
#define PARAM_PATH std::wstring(L"../../data/property/")
#endif

#ifdef _DEBUG
#define MAP_PATH std::string("data/map/")
#else
#define MAP_PATH std::string("../../data/map/")
#endif

#ifdef _DEBUG
#define ROOT_PATH std::wstring(L"data/")
#else
#define ROOT_PATH std::wstring(L"../../data/")
#endif