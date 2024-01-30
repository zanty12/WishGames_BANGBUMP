#include "ini.h"
#include <windows.h>

namespace ini {
	int GetInt(std::wstring filePath, std::wstring appName, std::wstring keyName, int iniVal) {
		return GetPrivateProfileInt(appName.c_str(), keyName.c_str(), iniVal, filePath.c_str());
	}

	int GetInt(std::wstring filePath, std::wstring appName, std::wstring keyName) {
		return GetInt(appName, keyName, filePath, 0);
	}


	float GetFloat(std::wstring filePath, std::wstring appName, std::wstring keyName, float iniVal) {
		auto str = GetString(filePath, appName, keyName, std::to_string(iniVal));
		return str.size() ? std::stof(str) : 0.0f;
	}

	float GetFloat(std::wstring filePath, std::wstring appName, std::wstring keyName) {
		return GetFloat(filePath, appName, keyName, 0.0f);
	}


	bool GetBool(std::wstring filePath, std::wstring appName, std::wstring keyName) {
		const unsigned int len = 512;
		wchar_t value[len] = {};
		std::wstring rst;
		GetPrivateProfileString(appName.c_str(), keyName.c_str(), L"", value, len, filePath.c_str());
		rst = value;
		return rst == L"true" || rst == L"TRUE" || rst == L"True";
	}

	std::string GetString(std::wstring filePath, std::wstring appName, std::wstring keyName, std::string iniVal) {
		const unsigned int len = 512;
		wchar_t wIniVal[len] = {};
		mbstowcs(wIniVal, iniVal.c_str(), len);

		wchar_t value[len] = {};
		char rst[len] = {};
		GetPrivateProfileString(appName.c_str(), keyName.c_str(), wIniVal, value, len, filePath.c_str());
		wcstombs(rst, value, len);
		return rst;
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