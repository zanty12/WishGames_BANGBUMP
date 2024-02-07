#include "multi_mode.h"
#include "multiplay.h"
#include "multi_ui.h"
#include "move_scene_anim.h"

void MultiPlayModeServerSide::UpdateStart(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {

}

void MultiPlayModeServerSide::UpdateResult(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {
	// ���ԃ��U���g�̌o�ߎ��Ԃ��v�Z
	float time = time_ - maxTime_ + resultTime_;
	
	// ���Ԃ��}�C�i�X�Ȃ�I���i�܂����ԃ��U���g�ł͂Ȃ��j
	if (time < 0.0f) return;

	// �͂��߂̂�
	if (preMode != mode && GetMode() != MULTI_MODE::CHARACTER_SELECT) {
		int maxScore = -1;
		int rank = 0;
		int preScore = -1;
		for (int i = 0; i < clients.size(); i++) {
			int score = maxScore;
			ServerPlayer *editPlayer = nullptr;

			// ���Z����Ă��Ȃ��N���C�A���g�ōő�X�R�A�̌���
			for (auto &client : clients) {
				if (client.second.player_->score <= score || score == -1) {
					score = client.second.player_->score;
					editPlayer = client.second.player_;
				}
			}

			if (editPlayer) {
				if (preScore != editPlayer->score) {
					rank = i;
				}

				int expRange = editPlayer->GetLvMaxSkillOrb() - editPlayer->GetLvMinSkillOrb();
				editPlayer->skillPoint += expRange * (1.0f - (float)rank / (float)clients.size());
				preScore = editPlayer->score;
			}

			maxScore = score;
		}
	}
}

void MultiPlayModeClientSide::DrawStart(RESPONSE_PLAYER &players, Vector2 offset) {
	float time = players.time;

	// �V�[���J�ڃA�j���[�V����
	


	const float SPAWN_ANIMATION_START_TIME = 5.0f;
	const float SPAWN_ANIMATION_TIME = 3.0f;
	float spawnSpanTime = SPAWN_ANIMATION_TIME / players.clients.size();	// �X�|�[��������Ԋu
	// �X�|�[��������
	if (MoveScene::Move(0.0f) && spawnSpanTime * clientSpawnCount <= time) {

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

	const float DROP_ANIMATION = 5.0f;



	rstSkillOrb.remove_if(
		[](ResultSkillOrb skillorb) {
			return skillorb.isDestroy;
		}
	);

	// �h���b�v�A�j���[�V����
	if (time <= DROP_ANIMATION) {

		// �����L���O�\�[�g
		auto sortPlayers = MultiPlayClient::clients;

		// �h���b�v�֐�
		auto dropOrb = [&](int rank, int dropNum, float velocity) {
			for (int i = 0; i < dropNum; i++) {
				float rad = MATH::Rand(-MATH::PI, MATH::PI);
				rstSkillOrb.push_back(ResultSkillOrb(
					CalcTimePosition(),
					CalcIconPosition(rank, sortPlayers.size()),
					Vector2(std::sin(rad), std::cosf(rad) * velocity)));
			}
			};

		// 0.5�b���ƂɃX�L���I�[�u���h���b�v
		if (500 < dropSkillOrbCoolTimer.GetNowTime()) {
			for (auto &kvp : sortPlayers) {
				auto client = kvp.second;
				int id = client->id;
				int rank = get_rank(sortPlayers, id);
				int dropNum = client->GetLvMaxSkillOrb() - client->GetLvMinSkillOrb();
				int dropRate = (1.0f - (float)(rank) / (float)players.clients.size());
				switch (rank) {
				case 0: dropOrb(rank, dropNum * 1.0f, 30.0f); break;
				case 1: dropOrb(rank, dropNum * 0.75f, 30.0f); break;
				case 2: dropOrb(rank, dropNum * 0.5f, 30.0f); break;
				case 3: dropOrb(rank, dropNum * 0.2f, 30.0f); break;
				}
				
			}
			dropSkillOrbCoolTimer.Start();
		}
	}
	else {
		MoveScene::Move(1.0f);
	}

	for (auto &skillOrb : rstSkillOrb) {
		skillOrb.Loop();
	}
}
