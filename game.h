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
	int timer_tex_ = -1;
	//skip first time collider update
	bool first_update_ = true;

	//start
	bool started_ = false;
	float start_timer_ = 3.5f;

	//Result
	int result_tex_ = 0;
	int button_a_ = 0;
	int button_b_ = 0;
	int button_yes_ = 0;
	int button_no_ = 0;
	int ui_training_ = 0;
	int black_back_ = 0;
	bool result_ = false;

	//bgm
	int practicemode_bgm_ = 0;

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

private:
	void UpdateNormal();
	void DrawNormal();
	void UpdateResult();
	void DrawResult();
};