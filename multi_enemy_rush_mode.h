#pragma once
#include "multi_mode.h"
#include "lib/collider2d.h"
#include "time.h"

/***********************************************************
	Server
************************************************************/
class MultiPlayEnemyRushModeServerSide : public MultiPlayModeServerSide {


public:
	MultiPlayEnemyRushModeServerSide()
		: MultiPlayModeServerSide(L"EnemyRush") {
	}
	void Update(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) override {
	}

	void CreateResponse(Storage &out) override { };

	MULTI_MODE GetMode(void) const override { return ENEMY_RUSH; }

};





/***********************************************************
	Client
************************************************************/
class MultiPlayEnemyRushModeClientSide : public MultiPlayModeClientSide {

public:
	MultiPlayEnemyRushModeClientSide() :
		MultiPlayModeClientSide(L"EnemyRush") {
		map_->backBGTexNo = LoadTexture(Asset::textures_.at(textures::bg_stage2_back));
		map_->middleBGTexNo = LoadTexture("data/texture/BG/BG_stage2_middle.png");
		map_->frontBGTexNo = LoadTexture(Asset::textures_.at(textures::bg_stage2_front));
		soNo = LoadSound("data/sound/BGM/stage2_BGM.wav");
		descTexNo = LoadTexture("data/texture/description/enemyrush_description.png");

		stageNameTexNo = LoadTexture("data/texture/UI/title/UI_stage2.png");
	};


	void Draw(RESPONSE_PLAYER &players, Vector2 offset) override {
		
	}

	void ParseResponse(Storage &in) override {

	}

	MULTI_MODE GetMode(void) const override { return ENEMY_RUSH; }
};
