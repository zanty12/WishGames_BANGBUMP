#pragma once
#include "collmngr.h"
#include "scene.h"
#include "player.h"
#include "mapmngr.h"
#include "projectilemngr.h"
#include "scenemngr.h"
#include "renderer.h"
#include "skillorbmngr.h"

class MapMngr;
class Player;
class Camera;
class SceneMngr;

class GameBase :public Scene
{
protected:
	std::list<Player *> players_;
	MapMngr *mapmngr_ = nullptr;
	Camera *camera_ = nullptr;

	SceneMngr *scene_mngr_ = nullptr;
	static Renderer *renderer_;
	static CollMngr *coll_mngr_;
	static ProjectileMngr *projectile_mngr_;
	static SkillOrbMngr *orb_mngr_;

public:
	GameBase() = delete;
	GameBase(SceneMngr *scene_mngr) : scene_mngr_(scene_mngr) {	}
	virtual ~GameBase();

	virtual void Update(void) {}
	virtual void Draw(void) {}

	static Renderer *GetRenderer() { return renderer_; }
	static CollMngr *GetCollMngr() { return coll_mngr_; }
	static ProjectileMngr *GetProjectileMngr() { return projectile_mngr_; }
	static SkillOrbMngr *GetSkillOrbMngr() { return orb_mngr_; }
	static void PutCellOther(int MAP_READ);

	std::list<Player *> &GetPlayers(void) { return players_; }
	Player *GetPlayer(void) { return players_.size() != 0 ? *players_.begin() : nullptr; }
};