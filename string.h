#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <string>

void ToString(wchar_t *src, char* dst, unsigned int len) {
	wcstombs(dst, src, len);
}
