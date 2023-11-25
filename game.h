#pragma once
#include "scene.h"
#include "player.h"
#include "mapmngr.h"

class MapMngr;
class Game :public Scene
{
private:
	Player* player_;
	MapMngr* mapmngr_;

public:
	Game();
	~Game() override {
		delete mapmngr_;
		delete player_;
	}
	void Update()override;
	void Draw()override;

	Player* GetPlayer() const {
		return player_;
	}
};