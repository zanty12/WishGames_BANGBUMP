#pragma once
#include <string>
#ifdef _DEBUG
#define PARAM_PATH std::wstring(L"")
#else
#define PARAM_PATH std::wstring(L"../../")
#endif