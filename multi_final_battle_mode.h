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
	MultiPlayFinalBattleModeServerSide() : MultiPlayModeServerSide(L"FinalBattle") { }

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
	MultiPlayFinalBattleModeClientSide() : MultiPlayModeClientSide(L"FinalBattle") {
		map_->backBGTexNo = LoadTexture(Asset::textures_.at(textures::bg_stage3_back));
		map_->middleBGTexNo = LoadTexture("data/texture/BG/BG_stage3_middle.png");
		map_->frontBGTexNo = LoadTexture(Asset::textures_.at(textures::bg_stage3_front));
		soNo = LoadSound("data/sound/BGM/stage3_BGM.wav");
		descTexNo = LoadTexture("data/texture/description/finalbattle_description.png");

		stageNameTexNo = LoadTexture("data/texture/UI/title/UI_finalstage.png");
	}


	void Draw(RESPONSE_PLAYER &players, Vector2 offset) override {

	}

	void ParseResponse(Storage &in) override {

	}

	MULTI_MODE GetMode(void) const override { return FINAL_BATTLE; }
};