#pragma once
#include "multi_mode.h"
#include "lib/collider2d.h"
#include "time.h"

class MultiPlayFinalBattleModeServerSide : public MultiPlayServerSide {


public:
	MultiPlayFinalBattleModeServerSide(GameBase *game)
		: MultiPlayServerSide(new MapMngr("data/map/MultiPlay_Map3.csv", game)) {
	}
	void Update(std::list<CLIENT_DATA_SERVER_SIDE> &clients) override {

	}

	void CreateResponse(Storage &out) override { };

	MULTI_MODE GetMode(void) const override { return FINAL_BATTLE; }

};




class MultiPlayFinalBattleModeClientSide : public MultiPlayClientSide {

public:
	MultiPlayFinalBattleModeClientSide(GameBase *game) : MultiPlayClientSide(new MapMngr("data/map/MultiPlay_Map3.csv", game)) {};


	void Draw(RESPONSE_PLAYER &players) override {

	}

	void ParseResponse(Storage &in) override {

	}

	MULTI_MODE GetMode(void) const override { return FINAL_BATTLE; }
};