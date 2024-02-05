#include "multi_mode.h"
#include "multiplay.h"
#include "multi_ui.h"

void MultiPlayModeServerSide::UpdateResult(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {
	// ���ԃ��U���g�̌o�ߎ��Ԃ��v�Z
	float time = time_ - maxTime_ - resultTime_;
	
	// ���Ԃ��}�C�i�X�Ȃ�I���i�܂����ԃ��U���g�ł͂Ȃ��j
	if (time < 0.0f) return;


}

void MultiPlayModeClientSide::DrawStart(RESPONSE_PLAYER &players, Vector2 offset) {
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

	const float RANKING_SORT_ANIMATION_TIME = 1.0f;

	rstSkillOrb.remove_if(
		[](ResultSkillOrb skillorb) {
			return skillorb.isDestroy;
		}
	);

	// �����L���O�\�[�g
	auto sortPlayers = players.clients;
	sort(sortPlayers);

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
		for (auto &client : sortPlayers) {
			int id = client.id;
			int rank = get_rank(sortPlayers, id);
			int dropRate = (1.0f - (float)(rank + 1) / (float)players.clients.size());
			dropOrb(rank, 20, 30.0f);
		}
		dropSkillOrbCoolTimer.Start();
	}

	for (auto &skillOrb : rstSkillOrb) {
		skillOrb.Loop();
	}
}
