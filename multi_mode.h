#pragma once
#include "multi_header.h"
#include "time.h"

class MultiPlayServerSide {
protected:
	float maxTime_ = 100.0f;
	float time_ = 0.0f;
	MapMngr *map_ = nullptr;


protected:
	virtual void GameUpdate(std::list<CLIENT_DATA_SERVER_SIDE>& clients) = 0;
public:
	MultiPlayServerSide(MapMngr *map) : map_(map) { }

	void Update(std::list<CLIENT_DATA_SERVER_SIDE> &clients) {
		if (maxTime_ < time_) {
			
		}
		else {
			time_ += Time::GetDeltaTime();
			GameUpdate(clients);
		}
	}

	virtual void CreateResponse(Storage& out) = 0;

	float GetTime(void) { return time_; }
	float GetMaxTime(void) { return maxTime_; }
};

class MultiPlayClientSide {
protected:
	MapMngr *map_ = nullptr;

public:
	MultiPlayClientSide(MapMngr *map) : map_(map) { }
	virtual void Draw(RESPONSE_PLAYER &players) = 0;
	virtual void ParseResponse(Storage& in) = 0;
};
