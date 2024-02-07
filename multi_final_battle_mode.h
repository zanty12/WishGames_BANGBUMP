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
	MultiPlayFinalBattleModeServerSide() : MultiPlayModeServerSide(new MultiMap(MAP_PATH + "MultiPlay_Map4.csv", MULTIPLAY_RUN_TYPE_SERVER), L"FinalBattle") { }

	void Update(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) override {
		for (auto client : clients) {
			auto player = client.second.player_;
			player->score = player->skillPoint;
		}
	}

	void CreateResponse(Storage &out) override { };

	MULTI_MODE GetMode(void) const override { return FINAL_BATTLE; }

};





/*******************************************************
  Client
********************************************************/
class MultiPlayFinalBattleModeClientSide : public MultiPlayModeClientSide {
public:
	MultiPlayFinalBattleModeClientSide() : MultiPlayModeClientSide(new MultiMap(MAP_PATH + "MultiPlay_Map4.csv", MULTIPLAY_RUN_TYPE_CLIENT), L"FinalBattle") {
		map_->backBGTexNo = LoadTexture(Asset::textures_.at(textures::bg_stage2_back));
		map_->frontBGTexNo = LoadTexture(Asset::textures_.at(textures::bg_stage2_front));
	}


	void Draw(RESPONSE_PLAYER &players, Vector2 offset) override {

	}

	void ParseResponse(Storage &in) override {

	}

	MULTI_MODE GetMode(void) const override { return FINAL_BATTLE; }
};