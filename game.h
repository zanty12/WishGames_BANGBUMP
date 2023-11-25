#pragma once
#include "scene.h"
#include "player.h"
#include "mapmngr.h"

class Game :public Scene
{
private:
	Player* player_;
	MapMngr* mapmngr_;

public:
	Game() {
		mapmngr_ = new MapMngr("data/map/1.csv");
		player_ = new Player(mapmngr_->GetPlayerSpawn(), 0.0f, LoadTexture("player.jpg"), Vector2(0.0f, 0.0f));

	}
	~Game() override {
		delete mapmngr_;
		delete player_;
	}
	void Update()override {
		mapmngr_->Update();
		player_->Update();

	}
	void Draw()override{
		mapmngr_->Draw();
		player_->Draw();
	}

};