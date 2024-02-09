#include "win_time.h"
#include <timeapi.h>


namespace WIN {
	void Time::Start(void) {
		nowTime = 0ul;
		Restart();
	}

	void Time::Stop(void) {
		nowTime += timeGetTime() - startTime;
		isActive = false;
	}

	void Time::Restart(void) {
		startTime = timeGetTime();
		isActive = true;
	}

	unsigned int Time::GetNowTime(void) {
		if (isActive)return timeGetTime() - startTime + nowTime;
		else return nowTime;		
	}
}
