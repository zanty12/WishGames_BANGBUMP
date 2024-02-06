#pragma once
#include "multi_mode.h"
#include "lib/collider2d.h"
#include "time.h"

class MultiPlayEnemyRushModeServerSide : public MultiPlayModeServerSide {


public:
	MultiPlayEnemyRushModeServerSide()
		: MultiPlayModeServerSide(new MultiMap(MAP_PATH + "MultiPlay_Map2.csv", MULTIPLAY_RUN_TYPE_SERVER), L"EnemyRush") {
	}
	void Update(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) override {
	}

	void CreateResponse(Storage &out) override { };

	MULTI_MODE GetMode(void) const override { return ENEMY_RUSH; }

};




class MultiPlayEnemyRushModeClientSide : public MultiPlayModeClientSide {

public:
	MultiPlayEnemyRushModeClientSide() :
		MultiPlayModeClientSide(new MultiMap(MAP_PATH + "MultiPlay_Map2.csv", MULTIPLAY_RUN_TYPE_CLIENT), L"EnemyRush") {
		map_->backBGTexNo = LoadTexture(Asset::textures_.at(textures::bg_stage2_back));
		map_->frontBGTexNo = LoadTexture(Asset::textures_.at(textures::bg_stage2_front));
	};


	void Draw(RESPONSE_PLAYER &players, Vector2 offset) override {
		
	}

	void ParseResponse(Storage &in) override {

	}

	MULTI_MODE GetMode(void) const override { return ENEMY_RUSH; }
};
