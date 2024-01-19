#pragma once
#include "gamebase.h"
//#include "collmngr.h"
//#include "scene.h"
//#include "player.h"
//#include "mapmngr.h"
//#include "projectilemngr.h"
//#include "scenemngr.h"
//#include "renderer.h"
//#include "skillorbmngr.h"

class MapMngr;
class Player;
class Camera;
class SceneMngr;
class GameBase;
class Scene;


class Game :public GameBase
{
private:
	//Player* player_;
	//std::list<Player*> player_list_;
	//MapMngr *mapmngr_;
	//Camera* camera_;

	//SceneMngr* scene_mngr_;
	//static Renderer* renderer_;
	//static CollMngr* coll_mngr_;
	//static ProjectileMngr* projectile_mngr_;
	//static SkillOrbMngr *orb_mngr_;
	float timer_ = 120.0f;

public:
	Game() = delete;
	Game(SceneMngr* scene_mngr);
	void Update()override;
	void Draw()override;

	Player *GetPlayer();
	//std::list<Player*> GetPlayerList() const {
	//	return player_list_;
	//}
	void DebugMenu() override;
	
	SceneMngr* GetSceneMngr() { return scene_mngr_; }

	

	//static Renderer* GetRenderer(){return renderer_;}
	//static CollMngr* GetCollMngr() {return coll_mngr_;}
	//static ProjectileMngr* GetProjectileMngr(){return projectile_mngr_;}
	//static SkillOrbMngr* GetSkillOrbMngr(){return orb_mngr_;}
};