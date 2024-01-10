#pragma once

class StorageLock {
private:
	int currentID = 0;
	int sentinelID = 0;

public:
	void Lock(void) {
		int id = currentID;
		currentID++;
		while (id != sentinelID);
	}

	void Unlock() {
		sentinelID++;
	}

	int ID(void) {
		return currentID;
	}
};