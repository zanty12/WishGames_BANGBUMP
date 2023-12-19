#pragma once
#include "collmngr.h"
#include "scene.h"
#include "player.h"
#include "mapmngr.h"
#include "scenemngr.h"

class MapMngr;
class Player;
class Camera;

class Renderer;

class Game :public Scene
{
private:
	Player *player_;
	MapMngr *mapmngr_;
	Camera* camera_;

	SceneMngr* scene_mngr_;
	static Renderer* renderer_;
	static CollMngr* coll_mngr_;

public:
	Game() = delete;
	Game(SceneMngr* scene_mngr);
	~Game() override;
	void Update()override;
	void Draw()override;

	Player *GetPlayer() const {
		return player_;
	}
	void DebugMenu() override;

	static Renderer* GetRenderer();
	static CollMngr* GetCollMngr();
};