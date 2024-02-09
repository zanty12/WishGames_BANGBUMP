#pragma once
#include "gamebase.h"
#include "multi_character_select_mode.h"
#include "multi_intermediate_result_mode.h"
#include "multi_area_capture_mode.h"
#include "multi_obstacle_race_mode.h"
#include "multi_enemy_rush_mode.h"
#include "multi_final_battle_mode.h"
#include "multi_last_result_mode.h"
#include "number.h"
#include "follow.h"

class MultiPlayServer;
class MultiPlayClient;

/***********************************************************
	Server
************************************************************/
class MultiPlayFlowServerSide {
private:
	MultiPlayModeServerSide *gameMode_ = nullptr;
	MultiPlayServer *game_ = nullptr;
	DWORD preTime = 0ul;



private:
	MultiPlayModeServerSide *CreateMode(MULTI_MODE mode);

public:
	MultiPlayFlowServerSide(MultiPlayServer* game) : game_(game) {
		gameMode_ = CreateMode(CHARACTER_SELECT);
	}
	~MultiPlayFlowServerSide() {
		if (gameMode_) delete gameMode_;
	}

	void Update(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients);

	void CreateResponse(Storage &out);

	MultiMap *GetMap(void) const { return gameMode_ ? gameMode_->map_ : nullptr; }
	MULTI_MODE GetMode(void) const { return gameMode_ ? gameMode_->GetMode() : MULTI_MODE::NONE; }
	float GetTime(void) const { return gameMode_ ? gameMode_->time_ : 0.0f; }
	float GetMaxTime(void) const { return gameMode_ ? gameMode_->maxTime_ : 0.0f; }
	MultiPlayModeServerSide *GetGame(void) const { return gameMode_; }
};






/***********************************************************
	Client
************************************************************/
class MultiPlayFlowClientSide {
private:
	MultiPlayModeClientSide *gameMode_ = nullptr;
	MultiPlayClient *game_ = nullptr;
	MULTI_MODE currentMode_ = MULTI_MODE::NONE;
	int numTexNo = LoadTexture("data/texture/UI/number.png");
	int timerTexNo = LoadTexture("data/texture/UI/multi_timer.png");
	int icon = -1;
	int icon2 = -1;
	int icon3 = -1;

private:
	MultiPlayModeClientSide *CreateMode(MULTI_MODE mode);
public:
	MultiPlayFlowClientSide(MultiPlayClient *game) : game_(game) {
		std::ostringstream path;
		path << "data/texture/UI/" << "UI_icon_" << (MultiPlayClient::GetID() + 1) % 4 << ".png";
		icon = LoadTexture(path.str());
		icon2 = LoadTexture("data/texture/UI/UI_icon_base.png");
		icon3 = LoadTexture("data/texture/UI/UI_icon_gauge.png");
	}

	~MultiPlayFlowClientSide() {
		if (gameMode_) delete gameMode_;
	}

	void Draw(RESPONSE_PLAYER &res, Vector2 offset);
	void DrawUI(RESPONSE_PLAYER &res);

	void ParseResponse(Storage &in);


	MultiMap *GetMap(void) const { return gameMode_ ? gameMode_->map_ : nullptr; }
	MULTI_MODE GetMode(void) const { return gameMode_ ? gameMode_->GetMode() : MULTI_MODE::NONE; }
};
