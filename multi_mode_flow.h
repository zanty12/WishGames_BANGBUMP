#pragma once
#include "gamebase.h"
#include "multi_character_select_mode.h"
#include "multi_intermediate_result_mode.h"
#include "multi_area_capture_mode.h"
#include "multi_enemy_rush_mode.h"

class MultiPlayFlowServerSide {
private:
	MultiPlayServerSide *gameMode_ = nullptr;
	GameBase *game_ = nullptr;



private:
	MultiPlayServerSide *CreateMode(MULTI_MODE mode) {
		switch (mode)
		{
		case CHARACTER_SELECT: return new MultiPlayCharacterSelectModeServerSide();
		case AREA_CAPTURE: return new MultiPlayAreaCaptureModeServerSide(game_);
		case INTERMEDIATE_RESULT_1: return new MultiPlayIntermediateResultModeServerSide();
		case OBSTACLE_RACE: return nullptr;
		case INTERMEDIATE_RESULT_2: return new MultiPlayIntermediateResultModeServerSide();
		case ENEMY_RUSH: return new MultiPlayEnemyRushModeServerSide(game_);
		case INTERMEDIATE_RESULT_3: return new MultiPlayIntermediateResultModeServerSide();
		case FINAL_BATTLE: return nullptr;
		}
		return nullptr;
	}

public:
	MultiPlayFlowServerSide(GameBase* game) : game_(game) {
		gameMode_ = CreateMode(AREA_CAPTURE);
	}

	void Update(std::list<CLIENT_DATA_SERVER_SIDE> &clients) {
		// ゲームモードがないなら終了
		if (gameMode_ == nullptr) return;

		// 制限時間が来たなら、次のモードへ移行
		if (gameMode_->maxTime_ < gameMode_->time_) {
			// 現在のモードの取得
			MULTI_MODE mode_ = GetMode();

			// 現在のモードの削除
			delete gameMode_;

			// 次のモードを計算
			mode_ = (MULTI_MODE)((int)mode_ + 1);

			// 次のモードの作成
			gameMode_ = CreateMode(mode_);
		}
		else {
			gameMode_->time_ += Time::GetDeltaTime();
			gameMode_->Update(clients);
		}
	}

	void CreateResponse(Storage &out) {
		if (gameMode_) {
			gameMode_->CreateResponse(out);
		}
	}

	MapMngr *GetMap(void) const { return gameMode_ ? gameMode_->map_ : nullptr; }
	MULTI_MODE GetMode(void) const { return gameMode_ ? gameMode_->GetMode() : MULTI_MODE::NONE; }
	float GetTime(void) const { return gameMode_ ? gameMode_->time_ : 0.0f; }
	float GetMaxTime(void) const { return gameMode_ ? gameMode_->maxTime_ : 0.0f; }
};

class MultiPlayFlowClientSide {
private:
	MultiPlayClientSide *gameMode_ = nullptr;
	GameBase *game_ = nullptr;
	MULTI_MODE previousMode_ = MULTI_MODE::NONE;



private:
	MultiPlayClientSide *CreateMode(MULTI_MODE mode) {
		switch (mode)
		{
		case CHARACTER_SELECT: return new MultiPlayCharacterSelectModeClientSide();
		case AREA_CAPTURE: return new MultiPlayAreaCaptureModeClientSide(game_);
		case INTERMEDIATE_RESULT_1: return new MultiPlayIntermediateResultModeClientSide();
		case OBSTACLE_RACE: return nullptr;
		case INTERMEDIATE_RESULT_2: return new MultiPlayIntermediateResultModeClientSide();
		case ENEMY_RUSH: return new MultiPlayEnemyRushModeClientSide(game_);
		case INTERMEDIATE_RESULT_3: return new MultiPlayIntermediateResultModeClientSide();
		case FINAL_BATTLE: return nullptr;
		}
		return nullptr;
	}
public:
	MultiPlayFlowClientSide(GameBase *game) : game_(game) {

	}

	void Draw(RESPONSE_PLAYER &res) {
		// モードが切り替わったなら、次のモードへ移行
		if (previousMode_ != res.mode) {
			// 現在のモードの取得
			MULTI_MODE mode_ = GetMode();

			// 現在のモードの削除
			delete gameMode_;

			// 次のモードを計算
			mode_ = (MULTI_MODE)((int)mode_ + 1);

			// 次のモードの作成
			gameMode_ = CreateMode(mode_);
		}

		if (gameMode_) {
			gameMode_->Draw(res);
			std::cout << res.mode << " : " << (int)res.time << std::endl;
		}
	}

	void ParseResponse(Storage &in) {
		if (gameMode_) {
			gameMode_->ParseResponse(in);
		}
	}


	MapMngr *GetMap(void) const { return gameMode_ ? gameMode_->map_ : nullptr; }
	MULTI_MODE GetMode(void) const { return gameMode_ ? gameMode_->GetMode() : MULTI_MODE::NONE; }
};