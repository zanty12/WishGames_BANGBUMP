#pragma once
#include "multi_mode.h"
#include "lib/collider2d.h"
#include "time.h"
#include "asset.h"

/*******************************************************
  Server
********************************************************/
class MultiPlayFinalBattleModeServerSide : public MultiPlayModeServerSide {
public:
	MultiPlayFinalBattleModeServerSide() : MultiPlayModeServerSide(new MultiMap("data/map/MultiPlay_Map4.csv")) { maxTime_ = 500; }

	void Update(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) override {

	}

	void CreateResponse(Storage &out) override { };

	MULTI_MODE GetMode(void) const override { return FINAL_BATTLE; }

};





/*******************************************************
  Client
********************************************************/
class MultiPlayFinalBattleModeClientSide : public MultiPlayModeClientSide {
public:
	MultiPlayFinalBattleModeClientSide() : MultiPlayModeClientSide(new MultiMap("data/map/MultiPlay_Map1.csv")) {
		map_->backBGTexNo = LoadTexture(Asset::textures_.at(textures::bg_stage2_back));
		map_->frontBGTexNo = LoadTexture(Asset::textures_.at(textures::bg_stage2_front));
	}


	void Draw(RESPONSE_PLAYER &players, Vector2 offset) override {

	}

	void ParseResponse(Storage &in) override {

	}

	MULTI_MODE GetMode(void) const override { return FINAL_BATTLE; }
};