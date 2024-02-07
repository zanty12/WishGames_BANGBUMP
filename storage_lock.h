#pragma once
#include <windows.h>
#include "load.h"

class StorageLock {
private:
	int currentID = 0;
	int sentinelID = 0;

public:
	void Lock(void) {
		int id = currentID;
		currentID++;
		while (id != sentinelID) {
			if (isNowLoad) Sleep(1);
		}
	}

	void Unlock() {
		sentinelID++;
	}

	int ID(void) {
		return currentID;
	}
};