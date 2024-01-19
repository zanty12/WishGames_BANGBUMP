#pragma once
#include "multi_header.h"
#include "multi_map.h"
#include "time.h"

class MultiPlayFlowServerSide;
class MultiPlayModeServerSide {
protected:
	float maxTime_ = 60.0f;
	float time_ = 0.0f;
	MultiMap *map_ = nullptr;
	friend MultiPlayFlowServerSide;


public:
	MultiPlayModeServerSide(MultiMap *map) : map_(map) {  }
	~MultiPlayModeServerSide() { if (map_) delete map_; }
	virtual void Update(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) { };
	virtual void CreateResponse(Storage& out) { };
	virtual void Release(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) { };

	virtual MULTI_MODE GetMode(void) const = 0;
	float GetTime(void) const { return time_; }
	float GetMaxTime(void) const { return maxTime_; }
	MultiMap *GetMap(void) { return map_; }
};




class MultiPlayFlowClientSide;
class MultiPlayModeClientSide {
protected:
	MultiMap *map_;
	friend MultiPlayFlowClientSide;

public:
	MultiPlayModeClientSide(MultiMap* map) : map_(map) { };
	~MultiPlayModeClientSide() { if (map_) delete map_; }

	virtual void Draw(RESPONSE_PLAYER &players, Vector2 offset) { };
	virtual void ParseResponse(Storage& in) { };
	virtual void Release(RESPONSE_PLAYER &players) { };


	virtual MULTI_MODE GetMode(void) const = 0;
	MultiMap *GetMap(void) { return map_; }
};
