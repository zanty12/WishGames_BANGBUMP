#pragma once
#include "multi_mode.h"
#include "lib/collider2d.h"
#include "time.h"

class MultiPlayObstacleRaceModeServerSide : public MultiPlayServerSide {


public:
	MultiPlayObstacleRaceModeServerSide(GameBase *game)
		: MultiPlayServerSide(new MapMngr("data/map/MultiPlay_Map2.csv", game)) {
	}
	void Update(std::list<CLIENT_DATA_SERVER_SIDE> &clients) override {

	}

	void CreateResponse(Storage &out) override { };

	MULTI_MODE GetMode(void) const override { return ENEMY_RUSH; }

};




class MultiPlayObstacleRaceModeClientSide : public MultiPlayClientSide {

public:
	MultiPlayObstacleRaceModeClientSide(GameBase *game) : MultiPlayClientSide(new MapMngr("data/map/MultiPlay_Map2.csv", game)) {};


	void Draw(RESPONSE_PLAYER &players) override {

	}

	void ParseResponse(Storage &in) override {

	}

	MULTI_MODE GetMode(void) const override { return INTERMEDIATE_RESULT_1; }
};