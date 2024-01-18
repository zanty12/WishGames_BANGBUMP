#pragma once
#include "multi_mode.h"
#include "lib/collider2d.h"
#include "time.h"

class MultiPlayFinalBattleModeServerSide : public MultiPlayModeServerSide {
public:
	MultiPlayFinalBattleModeServerSide() : MultiPlayModeServerSide(new MultiMap("data/map/MultiPlay_Map1.csv")) { }

	void Update(std::list<CLIENT_DATA_SERVER_SIDE> &clients) override {

	}

	void CreateResponse(Storage &out) override { };

	MULTI_MODE GetMode(void) const override { return FINAL_BATTLE; }

};




class MultiPlayFinalBattleModeClientSide : public MultiPlayModeClientSide {
public:
	MultiPlayFinalBattleModeClientSide() : MultiPlayModeClientSide(new MultiMap("data/map/MultiPlay_Map1.csv")) { }


	void Draw(RESPONSE_PLAYER &players) override {

	}

	void ParseResponse(Storage &in) override {

	}

	MULTI_MODE GetMode(void) const override { return FINAL_BATTLE; }
};