#pragma once
#include "collmngr.h"
#include "scene.h"
#include "player.h"
#include "mapmngr.h"
#include "projectilemngr.h"
#include "scenemngr.h"
#include "renderer.h"
#include "skillorbmngr.h"
#include "text.h"

class MapMngr;
class Player;
class Camera;
class SceneMngr;

class GameBase :public Scene
{
protected:
	static std::list<Player *> players_;
	MapMngr *mapmngr_ = nullptr;
	Camera *camera_ = nullptr;

	SceneMngr *scene_mngr_ = nullptr;
	static Renderer *renderer_;
	static CollMngr *coll_mngr_;
	static ProjectileMngr *projectile_mngr_;
	static SkillOrbMngr *orb_mngr_;
	IDWriteTextFormat* text_format_;
	ID2D1SolidColorBrush* brush_;

	int change_scene_;		//シーン遷移フラグ


private:
	constexpr static float LoD_threshold_ = 2500.0f;

public:
	GameBase() = delete;
	GameBase(SceneMngr *scene_mngr) : scene_mngr_(scene_mngr) { renderer_->InitRenderer(); }
	virtual ~GameBase();

	virtual void Update(void) {}
	virtual void Draw(void) {}

	static Renderer *GetRenderer() { return renderer_; }
	static CollMngr *GetCollMngr() { return coll_mngr_; }
	static ProjectileMngr *GetProjectileMngr() { return projectile_mngr_; }
	static SkillOrbMngr *GetSkillOrbMngr() { return orb_mngr_; }

	std::list<Player *> &GetPlayers(void) { return players_; }
	Player *GetPlayer(void) { return players_.size() != 0 ? *players_.begin() : nullptr; }
	void AddPlayer(Player *player) { players_.push_back(player); }
	void AddCamera(Camera *camera) { camera_ = camera; }
	MapMngr *GetMapMngr(void) { return mapmngr_; }
	static bool UpdateLoD(const Vector2 pos);
	SceneMngr* GetSceneMngr(void) { return scene_mngr_; }
	void SetChangeScene(int change_scene) { change_scene_ = change_scene; }
	int GetChangeScene(void) const { return change_scene_; }	//シーンチェンジのフラグ

};