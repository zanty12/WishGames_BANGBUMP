#pragma once
#include "gamebase.h"
#include "multi_character_select_mode.h"
#include "multi_intermediate_result_mode.h"
#include "multi_area_capture_mode.h"
#include "multi_obstacle_race_mode.h"
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
		case INTERMEDIATE_RESULT_1: return new MultiPlayIntermediateResult1ModeServerSide();
		case OBSTACLE_RACE: return new MultiPlayObstacleRaceModeServerSide(game_);
		case INTERMEDIATE_RESULT_2: return new MultiPlayIntermediateResult2ModeServerSide();
		case ENEMY_RUSH: return new MultiPlayEnemyRushModeServerSide(game_);
		case INTERMEDIATE_RESULT_3: return new MultiPlayIntermediateResult3ModeServerSide();
		case FINAL_BATTLE: return nullptr;
		}
		return nullptr;
	}

public:
	MultiPlayFlowServerSide(GameBase* game) : game_(game) {
		gameMode_ = CreateMode(AREA_CAPTURE);
	}

	void Update(std::list<CLIENT_DATA_SERVER_SIDE> &clients) {
		// �Q�[�����[�h���Ȃ��Ȃ�I��
		if (gameMode_ == nullptr) return;

		// �������Ԃ������Ȃ�A���̃��[�h�ֈڍs
		if (gameMode_->maxTime_ < gameMode_->time_) {
			// ���݂̃��[�h�̎擾
			MULTI_MODE mode_ = GetMode();

			// ���݂̃��[�h�̍폜
			delete gameMode_;
			gameMode_ = nullptr;

			// ���̃��[�h���v�Z
			mode_ = (MULTI_MODE)((int)mode_ + 1);

			// ���̃��[�h�̍쐬
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
	MultiPlayServerSide *GetGame(void) const { return gameMode_; }
};

class MultiPlayFlowClientSide {
private:
	MultiPlayClientSide *gameMode_ = nullptr;
	GameBase *game_ = nullptr;
	MULTI_MODE currentMode_ = MULTI_MODE::NONE;



private:
	MultiPlayClientSide *CreateMode(MULTI_MODE mode) {
		switch (mode)
		{
		case CHARACTER_SELECT: return new MultiPlayCharacterSelectModeClientSide();
		case AREA_CAPTURE: return new MultiPlayAreaCaptureModeClientSide(game_);
		case INTERMEDIATE_RESULT_1: return new MultiPlayIntermediateResult1ModeClientSide();
		case OBSTACLE_RACE: return new MultiPlayObstacleRaceModeClientSide(game_);
		case INTERMEDIATE_RESULT_2: return new MultiPlayIntermediateResult2ModeClientSide();
		case ENEMY_RUSH: return new MultiPlayEnemyRushModeClientSide(game_);
		case INTERMEDIATE_RESULT_3: return new MultiPlayIntermediateResult3ModeClientSide();
		case FINAL_BATTLE: return nullptr;
		}
		return nullptr;
	}
public:
	MultiPlayFlowClientSide(GameBase *game) : game_(game) {
	}
	~MultiPlayFlowClientSide() {
		if (gameMode_) delete gameMode_;
	}

	void Draw(RESPONSE_PLAYER &res) {
		std::cout << res.mode << std::endl;

		// ���[�h���؂�ւ�����Ȃ�A���̃��[�h�ֈڍs
		if (currentMode_ != res.mode) {
			// ���݂̃��[�h�̎擾
			MULTI_MODE mode_ = GetMode();

			// ���݂̃��[�h�̍폜
			if (gameMode_) delete gameMode_;

			// ���̃��[�h�̍쐬
			gameMode_ = CreateMode(res.mode);

			// ���̃��[�h�̎擾
			currentMode_ = GetMode();
		}
		// ���[�h�̎��s
		else if (gameMode_) {
			gameMode_->Draw(res);
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