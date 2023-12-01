#pragma once
#include "scene.h"
#include "player.h"
#include "mapmngr.h"

#include "scenemngr.h"

class MapMngr;
class Player;
class Camera;

class Game :public Scene
{
private:
	Player *player_;
	MapMngr *mapmngr_;
	Camera* camera_;

	SceneMngr* scene_mngr_;

public:
	Game() = delete;
	Game(SceneMngr* scene_mngr);
	~Game() override {
		delete mapmngr_;
		delete player_;
		delete camera_;
	}
	void Update()override;
	void Draw()override;

	Player *GetPlayer() const {
		return player_;
	}
	void DebugMenu() override;
};