#include "time.h"
DWORD Time::startTime = 0;
DWORD Time::previousTime = 0;
DWORD Time::currentTime = 0;
DWORD Time::deltaTime = 0;



void Time::Initialize(void) {
	startTime = timeGetTime();
	timeBeginPeriod(1000);
}

void Time::Release(void) {
	startTime = timeGetTime();
	timeBeginPeriod(1000);
}

void Time::Update(void) {
	previousTime = currentTime;
	currentTime = timeGetTime();
	deltaTime = currentTime - previousTime;
}

float Time::GetDeltaTime(void) {
	return deltaTime * 0.001f;
}