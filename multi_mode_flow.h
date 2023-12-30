#pragma once
#include "gamebase.h"
#include "multi_character_select_mode.h"
#include "multi_intermediate_result_mode.h"
#include "multi_area_capture_mode.h"
#include "multi_enemy_rush_mode.h"

class MultiPlayFlowServerSide {
private:
	MultiPlayServerSide *gameMode = nullptr;
	MapMngr *map_;

	union {
		MultiPlayServerSide *gameModes[6] = {};
		struct {
			MultiPlayCharacterSelectModeServerSide *characterSelect;
			MultiPlayIntermediateResultModeServerSide *intermediateResult;
			MultiPlayAreaCaptureModeServerSide *areaCapture;
			MultiPlayEnemyRushMode *enemyRush;
		};
	};

public:
	MultiPlayFlowServerSide(GameBase* game) {
		map_ = new MapMngr("data/map/MultiPlay_Map1.csv", game);
		areaCapture = new MultiPlayAreaCaptureModeServerSide(map_);
		gameMode = areaCapture;
	}

	void Update(std::list<CLIENT_DATA_SERVER_SIDE> &clients) {
		if (gameMode) {
			gameMode->Update(clients);
		}
	}

	void CreateResponse(Storage &out) {
		if (gameMode) {
			gameMode->CreateResponse(out);
		}
	}

	MapMngr *GetMap(void) const { return gameMode->map_; }
	float GetTime(void) const { return gameMode->time_; }
	float GetMaxTime(void) const { return gameMode->maxTime_; }
};

class MultiPlayFlowClientSide {
private:
	MultiPlayClientSide *gameMode = nullptr;
	MapMngr *map_;

	union {
		MultiPlayClientSide *gameModes[6] = {};
		struct {
			MultiPlayCharacterSelectModeClientSide *characterSelect;
			MultiPlayIntermediateResultModeClientSide *intermediateResult;
			MultiPlayAreaCaptureModeClientSide *areaCapture;
			MultiPlayEnemyRushMode *enemyRush;
		};
	};

public:
	MultiPlayFlowClientSide(GameBase *game) {
		map_ = new MapMngr("data/map/MultiPlay_Map1.csv", game);
		areaCapture = new MultiPlayAreaCaptureModeClientSide(map_);
		gameMode = areaCapture;
	}

	void Draw(RESPONSE_PLAYER &res) {
		if (gameMode) {
			gameMode->Draw(res);
		}
	}

	void ParseResponse(Storage &in) {
		if (gameMode) {
			gameMode->ParseResponse(in);
		}
	}


	MapMngr *GetMap(void) const { return gameMode->map_; }
};