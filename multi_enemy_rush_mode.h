#pragma once
#include "multi_mode.h"
#include "lib/collider2d.h"
#include "time.h"

class MultiPlayEnemyRushModeServerSide : public MultiPlayModeServerSide {


public:
	MultiPlayEnemyRushModeServerSide()
		: MultiPlayModeServerSide(new MultiMap(MAP_PATH + "MultiPlay_Map3.csv"), L"EnemyRush") {
	}
	void Update(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) override {
	}

	void CreateResponse(Storage &out) override { };

	MULTI_MODE GetMode(void) const override { return ENEMY_RUSH; }

};




class MultiPlayEnemyRushModeClientSide : public MultiPlayModeClientSide {

public:
	MultiPlayEnemyRushModeClientSide() :
		MultiPlayModeClientSide(new MultiMap(MAP_PATH + "MultiPlay_Map3.csv"), L"EnemyRush") {
	};


	void Draw(RESPONSE_PLAYER &players, Vector2 offset) override {
		
	}

	void ParseResponse(Storage &in) override {

	}

	MULTI_MODE GetMode(void) const override { return ENEMY_RUSH; }
};