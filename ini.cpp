#include "ini.h"
#include <windows.h>

namespace ini {
	int GetInt(std::wstring filePath, std::wstring appName, std::wstring keyName) {
		return GetPrivateProfileInt(appName.c_str(), keyName.c_str(), 0, filePath.c_str());
	}

	float GetFloat(std::wstring filePath, std::wstring appName, std::wstring keyName) {
		auto str = GetString(filePath, appName, keyName);
		return str.size() ? std::stof(str) : 0.0f;
	}

	bool GetBool(std::wstring filePath, std::wstring appName, std::wstring keyName) {
		const unsigned int len = 512;
		wchar_t value[len] = {};
		std::wstring rst;
		GetPrivateProfileString(appName.c_str(), keyName.c_str(), L"", value, len, filePath.c_str());
		rst = value;
		return rst == L"true" || rst == L"TRUE" || rst == L"True";
	}

	std::string GetString(std::wstring filePath, std::wstring appName, std::wstring keyName) {
		const unsigned int len = 512;
		wchar_t value[len] = {};
		char rst[len] = {};
		GetPrivateProfileString(appName.c_str(), keyName.c_str(), L"", value, len, filePath.c_str());
		wcstombs(rst, value, len);
		return rst;
	}
}