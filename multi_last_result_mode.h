#pragma once
#include "multi_mode.h"
#include "time.h"
#include "asset.h"


/*******************************************************
  Server
********************************************************/
class MultiPlayLastResultModeServerSide : public MultiPlayModeServerSide {
public:
	MultiPlayLastResultModeServerSide() : MultiPlayModeServerSide(L"LastResult") { }

	void Update(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) override {
		for (auto client : clients) {
			auto player = client.second.player_;
			player->score = player->skillPoint;
		}
	}

	void CreateResponse(Storage &out) override { };

	MULTI_MODE GetMode(void) const override { return LAST_RESULT; }

};





/*******************************************************
  Client
********************************************************/
class MultiPlayLastResultModeClientSide : public MultiPlayModeClientSide {
public:
	MultiPlayLastResultModeClientSide() : MultiPlayModeClientSide(L"LastResult") {
		map_->backBGTexNo = LoadTexture(Asset::textures_.at(textures::bg_stage3_back));
		map_->middleBGTexNo = LoadTexture("data/texture/BG/BG_stage3_middle.png");
		map_->frontBGTexNo = LoadTexture(Asset::textures_.at(textures::bg_stage3_front));
		soNo = LoadSound("data/sound/BGM/stage3_BGM.wav");
	}


	void Draw(RESPONSE_PLAYER &players, Vector2 offset) override {

	}

	void ParseResponse(Storage &in) override {

	}

	MULTI_MODE GetMode(void) const override { return LAST_RESULT; }
};