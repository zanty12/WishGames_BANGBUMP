#pragma once
#include "multi_header.h"
#include "time.h"

class MultiPlayFlowServerSide;
class MultiPlayServerSide {
protected:
	float maxTime_ = 100.0f;
	float time_ = 0.0f;
	MapMngr *map_ = nullptr;

	friend MultiPlayFlowServerSide;


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

	MapMngr *GetMap(void) const { return map_; }
	float GetTime(void) const { return time_; }
	float GetMaxTime(void) const { return maxTime_; }
};




class MultiPlayFlowClientSide;
class MultiPlayClientSide {
protected:
	MapMngr *map_ = nullptr;

	friend MultiPlayFlowClientSide;

public:
	MultiPlayClientSide(MapMngr *map) : map_(map) { }
	virtual void Draw(RESPONSE_PLAYER &players) = 0;
	virtual void ParseResponse(Storage& in) = 0;


	MapMngr *GetMap(void) const { return map_; }
};
