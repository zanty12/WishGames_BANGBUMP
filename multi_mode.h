#pragma once
#include "multi_header.h"
#include "time.h"

class MultiPlayServerSide {
protected:
	float maxTimeLimit_ = 100.0f;
	float timeLimit_ = maxTimeLimit_;
	MapMngr *map_ = nullptr;


protected:
	virtual void GameUpdate(std::list<CLIENT_DATA_SERVER_SIDE>& clients) = 0;
public:
	MultiPlayServerSide(MapMngr *map) : map_(map) { }

	void Update(std::list<CLIENT_DATA_SERVER_SIDE> &clients) {
		if (timeLimit_ <= 0.0f) {
			
		}
		else {
			timeLimit_ -= Time::GetDeltaTime();
			GameUpdate(clients);
		}
	}

	virtual void CreateResponse(Storage& out) = 0;

	float GetTimeLimit(void) { return timeLimit_; }
	float GetMaxTimeLimit(void) { return maxTimeLimit_; }
};

class MultiPlayClientSide {
protected:
	MapMngr *map_ = nullptr;

public:
	MultiPlayClientSide(MapMngr *map) : map_(map) { }
	virtual void Draw(RESPONSE_PLAYER &players) = 0;
	virtual void ParseResponse(Storage& in) = 0;
};
