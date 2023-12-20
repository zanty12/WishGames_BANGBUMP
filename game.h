#pragma once
#include "collmngr.h"
#include "scene.h"
#include "player.h"
#include "mapmngr.h"
#include "projectilemngr.h"
#include "scenemngr.h"
#include "renderer.h"

class MapMngr;
class Player;
class Camera;

class Game :public Scene
{
private:
	Player* player_;
	std::list<Player*> player_list_;
	MapMngr *mapmngr_;
	Camera* camera_;

	SceneMngr* scene_mngr_;
	static Renderer* renderer_;
	static CollMngr* coll_mngr_;
	static ProjectileMngr* projectile_mngr_;

public:
	Game() = delete;
	Game(SceneMngr* scene_mngr);
	~Game() override;
	void Update()override;
	void Draw()override;

	Player* GetPlayer() const {
		return player_;
	}
	std::list<Player*> GetPlayerList() const {
		return player_list_;
	}
	void DebugMenu() override;

	static Renderer* GetRenderer(){return renderer_;}
	static CollMngr* GetCollMngr() {return coll_mngr_;}
	static ProjectileMngr* GetProjectileMngr(){return projectile_mngr_;}
};