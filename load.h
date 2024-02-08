#pragma once
#include <thread>

class LoadClientSide {
public:
	static bool isNowLoad;
	static std::thread *thread;
	static void BeginLoad(void);
	static void EndLoad(void);

private:
	static void loading(void);
};

