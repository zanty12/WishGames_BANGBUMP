#pragma once
#include <windows.h>
#pragma comment(lib, "winmm.lib")
class Time {
private:
	static DWORD startTime;
	static DWORD previousTime;
	static DWORD currentTime;
	static DWORD deltaTime;

public:

	static void Initialize(void);
	static void Release(void);
	static void Update(void);
	static float GetDeltaTime(void);
	static float GetCurrentTime(void) { return currentTime * 0.001f; }
};