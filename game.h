#pragma once
#include "scene.h"
#include "player.h"
#include "mapmngr.h"

class MapMngr;
class Player;
class Camera;

class Game :public Scene
{
private:
	Player *player_;
	MapMngr *mapmngr_;
	Camera* camera_;

public:
	Game();
	~Game() override
	{
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