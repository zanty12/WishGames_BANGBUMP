#include "multiplay.h"
#include "multi_mode_flow.h"

/***********************************************************
	Server
************************************************************/
MultiPlayServerSide *MultiPlayFlowServerSide::CreateMode(MULTI_MODE mode) {
	startTime = timeGetTime();
	switch (mode)
	{
	case CHARACTER_SELECT: return new MultiPlayCharacterSelectModeServerSide(game_);
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

void MultiPlayFlowServerSide::Update(std::list<CLIENT_DATA_SERVER_SIDE> &clients) {
	// �Q�[�����[�h���Ȃ��Ȃ�I��
	if (gameMode_ == nullptr) return;

	// �������Ԃ������Ȃ�A���̃��[�h�ֈڍs
	if (gameMode_->maxTime_ < gameMode_->time_) {
		// ���݂̃��[�h�̎擾
		MULTI_MODE mode_ = GetMode();

		// ���݂̃��[�h�̍폜
		gameMode_->Release(clients);
		delete gameMode_;
		gameMode_ = nullptr;

		// ���̃��[�h���v�Z
		mode_ = (MULTI_MODE)((int)mode_ + 1);

		// ���̃��[�h�̍쐬
		gameMode_ = CreateMode(mode_);
	}
	else {
		float deltaTime = (timeGetTime() - startTime) * 0.001f;
		gameMode_->time_ = (timeGetTime() - startTime) * 0.001f;

		gameMode_->Update(clients);
	}
}

void MultiPlayFlowServerSide::CreateResponse(Storage &out) {
	if (gameMode_) {
		gameMode_->CreateResponse(out);
	}
}





/***********************************************************
	Client
************************************************************/
MultiPlayClientSide *MultiPlayFlowClientSide::CreateMode(MULTI_MODE mode) {
	switch (mode)
	{
	case CHARACTER_SELECT: return new MultiPlayCharacterSelectModeClientSide(game_);
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

void MultiPlayFlowClientSide::Draw(RESPONSE_PLAYER &res) {
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
		// ���Ԑ����̕`��
		Number(Vector2(100, 100), Vector2(100, 100), res.maxTime - res.time);

		// �X�R�A�̕`��
		int i = 0;
		for (auto &client : res.clients) {
			int moveAttribute = client.moveAttributeType;
			int attackAttribute = client.attackAttributeType;
			//�摜�̊֌W��Attack�����炷
			if (moveAttribute < attackAttribute) attackAttribute--;
			float u = moveAttribute / 4.0f;
			float v = attackAttribute / 12.0f;
			Vector2 uv = Vector2(u, v + i * 0.25f);

			DrawSprite(icon,
				Vector2(100 + i * 100, 100), 0.0f, Vector2(100, 100),
				Color::White,
				uv, Vector2(0.25f, 1.0f / 12.0f)
				);
			Number(Vector2(200, 200), Vector2(100, 100), client.skillPoint + 1);
			i++;
		}
	}
}

void MultiPlayFlowClientSide::ParseResponse(Storage &in) {
	if (gameMode_) {
		gameMode_->ParseResponse(in);
	}
}
