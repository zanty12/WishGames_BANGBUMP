#include "multiplay.h"
#include "multi_mode_flow.h"
#include "load.h"
#include "multi_ui.h"
#include "sound.h"
#include "lib/collider2d.h"

/***********************************************************
	Server
************************************************************/
MultiPlayModeServerSide *MultiPlayFlowServerSide::CreateMode(MULTI_MODE mode) {
	preTime = timeGetTime();
	MultiPlayModeServerSide *rstMode = nullptr;
	switch (mode)
	{
	case CHARACTER_SELECT: rstMode = new MultiPlayCharacterSelectModeServerSide(game_); break;
	case AREA_CAPTURE: rstMode = new MultiPlayAreaCaptureModeServerSide(); break;
	case ENEMY_RUSH: return new MultiPlayEnemyRushModeServerSide();
	case FINAL_BATTLE: rstMode = new MultiPlayFinalBattleModeServerSide(); break;
	case LAST_RESULT: rstMode = new MultiPlayLastResultModeServerSide(); break;
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
	if (gameMode_->maxTime_ < gameMode_->time_ || gameMode_->playTime_ <= 0.0f && gameMode_->isSkip) {
		MULTI_MODE mode_ = GetMode();
		SwapMode((MULTI_MODE)((int)mode_ + 1), clients);
		//// ���݂̃��[�h�̎擾
		//MULTI_MODE mode_ = GetMode();

		//// ���݂̃��[�h�̍폜
		//if (gameMode_) {
		//	// ���݂̃��[�h�̃����[�X�֐����Ăяo��
		//	gameMode_->Release(clients);

		//	delete gameMode_;
		//}

		//// ���̃��[�h���v�Z
		//mode_ = (MULTI_MODE)((int)mode_ + 1);

		//// ���̃��[�h�̍쐬
		//gameMode_ = CreateMode(mode_);

		//// �v���C���[�̈ړ�
		//if (gameMode_ && gameMode_->map_->startPosition.size()) {
		//	auto spawnPosIterator = gameMode_->map_->startPosition.begin();
		//	for (auto &client : clients) {
		//		// �ړ�����
		//		auto &player = client.second.player_;
		//		player->transform.position = *spawnPosIterator;

		//		// �X�^�[�g�n�_��1���炷
		//		spawnPosIterator++;

		//		// �����X�^�[�g�n�_���Ȃ��Ȃ�A���X�g�̍ŏ�����ɂ���
		//		if (gameMode_->map_->startPosition.end() == spawnPosIterator) {
		//			spawnPosIterator = gameMode_->map_->startPosition.begin();
		//		}
		//	}
		//}
	}
	else {
		// ���Ԃ̍X�V
		float deltaTime = Time::GetDeltaTime();
		if (gameMode_->startTime_ <= gameMode_->time_ && gameMode_->playTime_ < 0.0f && !gameMode_->isSkip) deltaTime = 0.0f;
		gameMode_->time_ += deltaTime;

		float absPlayTime = MATH::Abs(gameMode_->playTime_);

		// �Q�[���̃X�^�[�g�̍X�V
		if (gameMode_->time_ < gameMode_->startTime_) {
			gameMode_->mode = MultiPlayModeServerSide::START;
			gameMode_->UpdateStart(clients);
		}
		// �Q�[�����[�h�̍X�V
		else if (gameMode_->time_ < gameMode_->startTime_ + absPlayTime ||
			gameMode_->playTime_ <= 0.0f && gameMode_->isSkip == false) {
			gameMode_->mode = MultiPlayModeServerSide::PLAY;
			gameMode_->Update(clients);

			if (GetAsyncKeyState(VK_RETURN)) {
				if (0.0f <= gameMode_->playTime_)gameMode_->time_ = gameMode_->startTime_ + gameMode_->playTime_;
			}
		}
		// �Q�[���̃��U���g�̍X�V
		else {
			gameMode_->mode = MultiPlayModeServerSide::RESULT;
			gameMode_->UpdateResult(clients);
		}
		gameMode_->preMode = gameMode_->mode;
		preTime = timeGetTime();
	}
}

void MultiPlayFlowServerSide::CreateResponse(Storage &out) {
	if (gameMode_) {
		gameMode_->CreateResponse(out);
	}
}

void MultiPlayFlowServerSide::SwapMode(MULTI_MODE mode, std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {

	// ���݂̃��[�h�̍폜
	if (gameMode_) {
		// ���݂̃��[�h�̃����[�X�֐����Ăяo��
		gameMode_->Release(clients);

		delete gameMode_;
	}

	// ���̃��[�h�̍쐬
	gameMode_ = CreateMode(mode);

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





/***********************************************************
	Client
************************************************************/
MultiPlayModeClientSide *MultiPlayFlowClientSide::CreateMode(MULTI_MODE mode) {
	switch (mode)
	{
	case CHARACTER_SELECT: return new MultiPlayCharacterSelectModeClientSide(game_);
	case AREA_CAPTURE: return new MultiPlayAreaCaptureModeClientSide();
	case ENEMY_RUSH: return new MultiPlayEnemyRushModeClientSide();
	case FINAL_BATTLE: return new MultiPlayFinalBattleModeClientSide();
	case LAST_RESULT: return new MultiPlayLastResultModeClientSide(game_);
	}
	return nullptr;
}

MultiPlayFlowClientSide::MultiPlayFlowClientSide(MultiPlayClient *game) : game_(game) {
	icon[0] = LoadTexture("data/texture/UI/UI_icon_1.png");
	icon[1] = LoadTexture("data/texture/UI/UI_icon_2.png");
	icon[2] = LoadTexture("data/texture/UI/UI_icon_3.png");
	icon[3] = LoadTexture("data/texture/UI/UI_icon_4.png");
	icon2 = LoadTexture("data/texture/UI/UI_icon_base.png");
	icon3 = LoadTexture("data/texture/UI/UI_icon_gauge.png");
}

void MultiPlayFlowClientSide::Draw(RESPONSE_PLAYER &res, Vector2 offset) {

	// ���[�h���؂�ւ�����Ȃ�A���̃��[�h�ֈڍs
	if (currentMode_ != res.mode) {
		// ���݂̃��[�h�̎擾
		MULTI_MODE mode_ = GetMode();

		// ���݂̃��[�h�̍폜
		if (gameMode_) {
			// BGM�𗬂�
			if (gameMode_->soNo != -1) StopSound(gameMode_->soNo);

			// ���݂̃��[�h�̃����[�X�֐����Ăяo��
			gameMode_->Release(res);
			
			delete gameMode_;
		}

		// ���̃��[�h�̍쐬
		gameMode_ = CreateMode(res.mode);

		// ���̃��[�h�̎擾
		currentMode_ = GetMode();

		// BGM�𗬂�
		if (gameMode_ && gameMode_->soNo != -1) {
			PlaySound(gameMode_->soNo, -1);
			SetVolume(gameMode_->soNo, 0.4f);
		}
	}
	// ���[�h�̎��s
	else if (gameMode_) {

		// �}�b�v�̕`��
		gameMode_->map_->Draw(offset, gameMode_->isBlockShow);

		// �Q�[���̃X�^�[�g�̉��
		if (res.time < gameMode_->startTime_) {
			gameMode_->DrawStart(res, offset);
		}
		// �Q�[���̃��U���g�̕`��
		else if (0.0f < res.time - res.maxTime + gameMode_->resultTime_) {
			gameMode_->DrawResult(res, offset);
		}
		// �Q�[�����[�h�̕`��
		else {
			gameMode_->Draw(res, offset);
		}
	}
}

void MultiPlayFlowClientSide::DrawUI(RESPONSE_PLAYER &res) {
	if (gameMode_ == nullptr) return;

	// �Q�[�����[�h�̕`��iUI�j
	gameMode_->DrawUI(res);

	// �v���C���Ԃ��Ȃ��i�L�����I�� or ���U���g�j
	if (gameMode_->playTime_ < 0.0f) return;




	// �J�[�\���̕\��
	auto player = MultiPlayClient::clients[MultiPlayClient::GetID()];
	using namespace PHYSICS;
	for (auto kvp : MultiPlayClient::clients) {
		auto other = kvp.second;

		// �������g�Ȃ�J�[�\������Ȃ�
		if (other->id == player->id) continue;

		float bottom = MultiPlayClient::offset.y;
		float upper = bottom + Graphical::GetHeight();
		Vector2 direction = other->transform.position - player->transform.position;
		Vector2 scl = Vector2(75, 75);
		float rot = std::atan2(direction.x, direction.y);

		{
			float n = upper - player->transform.position.y;
			float m = other->transform.position.y - upper;
			if (0.0f < m) {
				Vector2 pos = (other->transform.position * n + player->transform.position * m) / (n + m);
				pos -= direction.Normalize() * 10.0f;

				DrawSprite(other->cursorTexNo, pos - MultiPlayClient::offset, rot, scl, Color::White);
			}
		}
		{
			float n = player->transform.position.y - bottom;
			float m = bottom - other->transform.position.y;
			if (0.0f < m) {
				Vector2 pos = (other->transform.position * n + player->transform.position * m) / (n + m);
				pos -= direction.Normalize() * 10.0f;

				DrawSprite(other->cursorTexNo, pos - MultiPlayClient::offset, rot, scl, Color::White);
			}
		}
	}





	float centerX = Graphical::GetWidth() * 0.5f;		// ��ʂ̒����iX���W�j

	// ���Ԑ����̕`��iUI�j
	DrawSprite(
		timerTexNo,
		CalcTimePosition(), 0.0f, Vector2(1000.0f, 300.0f) * 0.625f,
		Color::White,
		Vector2::Zero, Vector2::One
	);
	// ���Ԑ����̕`��i���l�j
	// �Q�[���̃X�^�[�g
	if (res.time < gameMode_->startTime_) {
		Number(Vector2(centerX, 107.0f), Vector2(75, 75), gameMode_->startTime_ - res.time);
	}
	// �Q�[���̃��U���g
	else if (0.0f < res.time - res.maxTime + gameMode_->resultTime_) {
		//Number(Vector2(centerX, 107.0f), Vector2(75, 75), res.maxTime - res.time);
	}
	// �Q�[�����[�h
	else {
		Number(Vector2(centerX, 107.0f), Vector2(75, 75), res.maxTime - gameMode_->resultTime_ - res.time);
	}


	//// �J�E���g�_�E��
	//float countDown = startTime_ - time;
	//if (countDown < 4.0f) {
	//	float vh = 1.0f / 4.0f;
	//	float v = (int)countDown * vh;
	//	float centerX = Graphical::GetWidth() * 0.5f;
	//	float centerY = Graphical::GetHeight() * 0.5f;
	//	float t = countDown - (int)countDown;
	//	float rate = MATH::Leap(0.4f, 1.0f, t * t);

	//	DrawSprite(countDownTexNo, Vector2(centerX, centerY), 0.0f, Vector2(800, 800) * rate,
	//		Color(1, 1, 1, rate), Vector2(0.0f, v), Vector2(1.0, vh));
	//}

	// �X�R�A�̕`��
	int maxMembers = res.clients.size();				// �v���C���[�l��
	for (auto &client : res.clients) {
		int idx = client.id;
		Vector2 uv = Vector2::Zero;
		Vector2 uvScale = Vector2::One;
		Vector2 pos = CalcIconPosition(idx, maxMembers);
		Vector2 scl = Vector2(300, 300);
		DrawSprite(icon2,
			pos, 0.0f, scl,
			Color::White,
			uv, uvScale
		);
		{
			auto player = MultiPlayClient::clients[client.id];
			// �X�L���|�C���g�֘A�̎擾
			int curSkillOrb = player->skillPoint;
			int minSkillOrb = player ? player->GetLvMinSkillOrb() : 0;
			int maxSkillOrb = player ? player->GetLvMaxSkillOrb() : 0;
			// �Q�[�W�̊������v�Z
			float ratio = (float)(curSkillOrb - minSkillOrb) / (float)(maxSkillOrb - minSkillOrb);
			if (ratio < 0.0f) ratio = 0.0f;
			else if (ratio > 1.0f) ratio = 1.0f;
			player->skillPointAnimation += (ratio - player->skillPointAnimation) * 1.0f;
			// UV�l
			float t = MATH::Leap(0.25f, 0.98f, ratio);
			

			float x = pos.x - scl.x * 0.5f;
			//DrawSpriteBoxEffectLeftToRight(
			//	icon3, pos, scl, Color::White, t
			//);
			DrawSprite(icon3,
				Vector2(x + scl.x * t * 0.5f, pos.y), 0.0f, Vector2(scl.x * t, scl.y),
				Color::White,
				uv, Vector2(t, 1.0f)
			);
		};
		DrawSprite(icon[idx % 4],
			pos, 0.0f, scl,
			Color::White,
			uv, uvScale
		);

		// �X�R�A
		float centerX = Graphical::GetWidth() * 0.5f;
		float height = Graphical::GetHeight();
		switch (idx) {
		case 0: Number(Vector2(centerX - 215, 78.0f), Vector2::One * 60.0f, 100/*client.score*/); break;
		case 1: Number(Vector2(centerX -  97, 83.0f), Vector2::One * 60.0f, 100/*client.score*/); break;
		case 2: Number(Vector2(centerX +  97, 83.0f), Vector2::One * 60.0f, client.score); break;
		case 3: Number(Vector2(centerX + 215, 78.0f), Vector2::One * 60.0f, client.score); break;
		}
	}
}

void MultiPlayFlowClientSide::ParseResponse(Storage &in) {
	if (gameMode_) {
		gameMode_->ParseResponse(in);
	}
}
