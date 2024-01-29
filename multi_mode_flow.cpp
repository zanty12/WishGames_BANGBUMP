#include "multiplay.h"
#include "multi_mode_flow.h"

/***********************************************************
	Server
************************************************************/
MultiPlayModeServerSide *MultiPlayFlowServerSide::CreateMode(MULTI_MODE mode) {
	startTime = timeGetTime();
	MultiPlayModeServerSide *rstMode = nullptr;
	switch (mode)
	{
	case CHARACTER_SELECT: rstMode = new MultiPlayCharacterSelectModeServerSide(game_); break;
	case AREA_CAPTURE: rstMode = new MultiPlayAreaCaptureModeServerSide(); break;
	//case INTERMEDIATE_RESULT_1: return new MultiPlayIntermediateResult1ModeServerSide();
	//case OBSTACLE_RACE: return new MultiPlayObstacleRaceModeServerSide(game_);
	//case INTERMEDIATE_RESULT_2: return new MultiPlayIntermediateResult2ModeServerSide();
	//case ENEMY_RUSH: return new MultiPlayEnemyRushModeServerSide(game_);
	//case INTERMEDIATE_RESULT_3: return new MultiPlayIntermediateResult3ModeServerSide();
	case FINAL_BATTLE: rstMode = new MultiPlayFinalBattleModeServerSide(); break;
	}

	// ���݂���}�b�v�̍X�V
	if (rstMode) {
		for (auto &client : MultiPlayServer::clients_) client.second.player_->map = rstMode->map_;
	}

	return rstMode;
}

void MultiPlayFlowServerSide::Update(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {
	// �Q�[�����[�h���Ȃ��Ȃ�I��
	if (gameMode_ == nullptr) return;

	// �������Ԃ������Ȃ�A���̃��[�h�ֈڍs
	if (gameMode_->maxTime_ < gameMode_->time_ || gameMode_->isSkip) {
		// ���݂̃��[�h�̎擾
		MULTI_MODE mode_ = GetMode();

		// ���݂̃��[�h�̍폜
		if (gameMode_) {
			// ���݂̃��[�h�̃����[�X�֐����Ăяo��
			gameMode_->Release(clients);

			delete gameMode_;
		}

		// ���̃��[�h���v�Z
		mode_ = (MULTI_MODE)((int)mode_ + 1);

		// ���̃��[�h�̍쐬
		gameMode_ = CreateMode(mode_);

		// �v���C���[�̈ړ�
		if (gameMode_ && gameMode_->map_->startPosition.size()) {
			auto spawnPosIterator = gameMode_->map_->startPosition.begin();
			for (auto &client : clients) {
				// �ړ�����
				auto &player = client.second.player_;
				player->transform.position = *spawnPosIterator;

				// �X�^�[�g�n�_��1���炷
				spawnPosIterator++;

				// �����X�^�[�g�n�_���Ȃ��Ȃ�A���X�g�̍ŏ�����ɂ���
				if (gameMode_->map_->startPosition.end() == spawnPosIterator) {
					spawnPosIterator = gameMode_->map_->startPosition.begin();
				}
			}
		}
	}
	else {
		// ���Ԃ̍X�V
		float deltaTime = (timeGetTime() - startTime) * 0.001f;
		gameMode_->time_ = deltaTime;



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
MultiPlayModeClientSide *MultiPlayFlowClientSide::CreateMode(MULTI_MODE mode) {
	switch (mode)
	{
	case CHARACTER_SELECT: return new MultiPlayCharacterSelectModeClientSide(game_);
	case AREA_CAPTURE: return new MultiPlayAreaCaptureModeClientSide();
	//case INTERMEDIATE_RESULT_1: return new MultiPlayIntermediateResult1ModeClientSide();
	//case OBSTACLE_RACE: return new MultiPlayObstacleRaceModeClientSide(game_);
	//case INTERMEDIATE_RESULT_2: return new MultiPlayIntermediateResult2ModeClientSide();
	//case ENEMY_RUSH: return new MultiPlayEnemyRushModeClientSide(game_);
	//case INTERMEDIATE_RESULT_3: return new MultiPlayIntermediateResult3ModeClientSide();
	case FINAL_BATTLE: return new MultiPlayFinalBattleModeClientSide();
	}
	return nullptr;
}

void MultiPlayFlowClientSide::Draw(RESPONSE_PLAYER &res, Vector2 offset) {

	// ���[�h���؂�ւ�����Ȃ�A���̃��[�h�ֈڍs
	if (currentMode_ != res.mode) {
		// ���݂̃��[�h�̎擾
		MULTI_MODE mode_ = GetMode();

		// ���݂̃��[�h�̍폜
		if (gameMode_) {
			// ���݂̃��[�h�̃����[�X�֐����Ăяo��
			gameMode_->Release(res);
			
			delete gameMode_;
		}

		// ���̃��[�h�̍쐬
		gameMode_ = CreateMode(res.mode);

		// ���̃��[�h�̎擾
		currentMode_ = GetMode();
	}
	// ���[�h�̎��s
	else if (gameMode_) {

		// �}�b�v�̕`��
		gameMode_->map_->Draw(offset);



		// �Q�[���̃��U���g�̕`��
		std::cout << res.time << " - " << gameMode_->resultTime_ << " = " << res.time - gameMode_->resultTime_ << std::endl;
		if (0.0f < res.time - gameMode_->resultTime_) {
			gameMode_->DrawResult(res, offset);
		}
		// �Q�[�����[�h�̕`��
		else {
			gameMode_->Draw(res, offset);
		}


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
			Number(Vector2(200 * (i + 1), 200), Vector2(100, 100), client.skillPoint + 1);
			i++;
		}
	}
}

void MultiPlayFlowClientSide::ParseResponse(Storage &in) {
	if (gameMode_) {
		gameMode_->ParseResponse(in);
	}
}
