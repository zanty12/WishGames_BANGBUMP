#include "multi_mode.h"
#include "multiplay.h"
#include "multi_ui.h"
#include "move_scene_anim.h"

void MultiPlayModeServerSide::UpdateStart(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {
	// �J�n�̌o�ߎ��Ԃ��v�Z
	float time = time_;

	isPlayerMove = 5.0f < time;
}

void MultiPlayModeServerSide::UpdateResult(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {
	// ���ԃ��U���g�̌o�ߎ��Ԃ��v�Z
	float time = time_ - maxTime_ + resultTime_;
	
	// ���Ԃ��}�C�i�X�Ȃ�I���i�܂����ԃ��U���g�ł͂Ȃ��j
	if (time < 0.0f) return;

	// �ړ��ł��Ȃ��悤�ɂ���
	isPlayerMove = false;

	// �͂��߂̂�
	if (preMode != mode && GetMode() != MULTI_MODE::CHARACTER_SELECT) {
		// �����L���O
		std::list<ServerPlayer *> ranking;
		for (auto &kvp : MultiPlayServer::clients_) ranking.push_back(kvp.second.player_);

		// �\�[�g
		ranking.sort([](ServerPlayer *a, ServerPlayer *b) {
			return a->score > b->score;
			}
		);


		int rank = 0;
		int preScore = -1;
		int addRank = 1;
		for (auto player : ranking) {
			if (player->score == preScore) addRank++;
			else addRank = 1;

			int expRange = player->GetLvMaxSkillOrb() - player->GetLvMinSkillOrb();
			switch (rank) {
			case 0: player->skillPoint += expRange * 0.7f; break;
			case 1: player->skillPoint += expRange * 0.5f; break;
			case 2: player->skillPoint += expRange * 0.2f; break;
			case 3: player->skillPoint += expRange * 0.0f; break;
			}

			rank += addRank;
			preScore = player->score;
		}
	}
}

void MultiPlayModeClientSide::DrawStart(RESPONSE_PLAYER &players, Vector2 offset) {
	float time = players.time;


	const float SPAWN_ANIMATION_START_TIME = 2.0f;
	const float SPAWN_ANIMATION_TIME = 3.0f;
	float spawnSpanTime = SPAWN_ANIMATION_TIME / players.clients.size();	// �X�|�[��������Ԋu

	// �X�|�[��������
	if (MoveScene::Move(Color::White * 0.0f) && SPAWN_ANIMATION_START_TIME + spawnSpanTime * clientSpawnCount <= time) {

		// �C�e���[�^
		auto iterator = players.clients.begin();

		// �A�j���[�V��������v���C���[�̃C�e���[�^
		for (int i = 0; i < clientSpawnCount && iterator != players.clients.end(); i++) iterator++;

		// �C�e���[�^�����݂���Ȃ�
		if (iterator != players.clients.end()) {
			// �A�j���[�V��������
			MultiPlayClient::clients[iterator->id]->ShowEntry();
		}
		clientSpawnCount++;
	}
}

void MultiPlayModeClientSide::DrawResult(RESPONSE_PLAYER &players, Vector2 offset) {
	// ���ԃ��U���g�̌o�ߎ��Ԃ��v�Z
	float time = players.time - players.maxTime + resultTime_;

	// ���Ԃ��}�C�i�X�Ȃ�I���i�܂����ԃ��U���g�ł͂Ȃ��j
	if (time < 0.0f) return;

	const float FADE_ANIMATION = 0.5f;
	const float DROP_ANIMATION = 2.5f;
	const float STAY_ANIMATION = 3.5f;
	const float WARP_ANIMATION = 9.5f;



	rstSkillOrb.remove_if(
		[](ResultSkillOrb skillorb) {
			return skillorb.isDestroy;
		}
	);
	if (time <= FADE_ANIMATION) {
		MoveScene::Move(Color::Black * 0.5f);
		clientSpawnCount = 0;
	}

	// �h���b�v�A�j���[�V����
	else if (time <= DROP_ANIMATION) {
		// �����L���O
		std::list<ClientPlayer *> ranking;
		for (auto &kvp : MultiPlayClient::clients) ranking.push_back(kvp.second);

		// �\�[�g
		ranking.sort([](ClientPlayer *a, ClientPlayer *b) {
			return a->score > b->score;
			}
		);

		// �h���b�v�֐�
		auto dropOrb = [&](int idx, int dropNum, float velocity) {
			for (int i = 0; i < dropNum; i++) {
				float rad = MATH::Rand(-MATH::PI, MATH::PI);
				rstSkillOrb.push_back(ResultSkillOrb(
					CalcTimePosition(),
					CalcIconPosition(idx, ranking.size()),
					Vector2(std::sin(rad), std::cosf(rad) * velocity)));
			}
		};

		// 0.5�b���ƂɃX�L���I�[�u���h���b�v
		if (100 < dropSkillOrbCoolTimer.GetNowTime()) {
			int rank = 0;
			int preScore = -1;
			int addRank = 1;
			int dropNum = 5;
			for (auto player : ranking) {
				if (player->score == preScore) addRank++;
				else addRank = 1;

				int expRange = player->GetLvMaxSkillOrb() - player->GetLvMinSkillOrb();
				switch (rank) {
				case 0: dropOrb(player->id, dropNum * 1.5f, 30.0f); break;
				case 1: dropOrb(player->id, dropNum * 0.5f, 30.0f); break;
				case 2: dropOrb(player->id, dropNum * 0.25f, 30.0f); break;
				case 3: dropOrb(player->id, dropNum * 0.0f, 30.0f); break;
				}
				rank += addRank;
				preScore = player->score;
			}
			dropSkillOrbCoolTimer.Start();
		}
	}
	else if (time <= STAY_ANIMATION) {
	}
	else if (time <= WARP_ANIMATION) {
		float time = players.time;


		const float SPAWN_ANIMATION_TIME = 3.0f;
		float spawnSpanTime = SPAWN_ANIMATION_TIME / players.clients.size();	// �X�|�[��������Ԋu

		// �X�|�[��������
		if (spawnSpanTime * clientSpawnCount <= time) {

			// �C�e���[�^
			auto iterator = players.clients.begin();

			// �A�j���[�V��������v���C���[�̃C�e���[�^
			for (int i = 0; i < clientSpawnCount && iterator != players.clients.end(); i++) iterator++;

			// �C�e���[�^�����݂���Ȃ�
			if (iterator != players.clients.end()) {
				// �A�j���[�V��������
				MultiPlayClient::clients[iterator->id]->ShowExit();
			}
			clientSpawnCount++;
		}
	}
	else {
		MoveScene::Move(Color::White * 1.0f);
	}

	for (auto &skillOrb : rstSkillOrb) {
		skillOrb.Loop();
	}
}
