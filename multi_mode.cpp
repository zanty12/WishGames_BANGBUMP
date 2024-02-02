#include "multi_mode.h"

void MultiPlayModeServerSide::UpdateResult(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {
	// ���ԃ��U���g�̌o�ߎ��Ԃ��v�Z
	float time = time_ - maxTime_ - resultTime_;
	
	// ���Ԃ��}�C�i�X�Ȃ�I���i�܂����ԃ��U���g�ł͂Ȃ��j
	if (time < 0.0f) return;


}

void MultiPlayModeClientSide::DrawResult(RESPONSE_PLAYER &players, Vector2 offset) {
	// ���ԃ��U���g�̌o�ߎ��Ԃ��v�Z
	float time = players.time - players.maxTime + resultTime_;

	// ���Ԃ��}�C�i�X�Ȃ�I���i�܂����ԃ��U���g�ł͂Ȃ��j
	if (time < 0.0f) return;

	auto afterClients = players.clients;
	const float RANKING_SORT_ANIMATION_TIME = 1.0f;

	// �����L���O�̃A�j���[�V����
	if (time <= RANKING_SORT_ANIMATION_TIME) {

		// �����L���O���\�[�g����
		sort(beforeClients);									// �Q�[���J�n���̃����L���O�\
		sort(afterClients);										// ���݂̃����L���O�\

		int playerNum = players.clients.size();					// �v���C���[��
		float t = time / RANKING_SORT_ANIMATION_TIME;			// ���Ԃ̊���
		t *= t;													// �񎟊֐��ɕϊ�������


		for (auto &client : players.clients) {
			int id = client.id;									// ID
			int preRank = get_rank(beforeClients, id);			// 1�Q�[���O�̃����N
			int rank = get_rank(afterClients, id);				// ���݂̃����N

			float height = 100.0f;								// �L�����N�^�[�G�̍���
			float y = rank - playerNum * 0.5f;					// ���݂̃����L���O��Y���W
			float preY = preRank - playerNum * 0.5f;			// 1�Q�[���O�̃����L���O��Y���W
			Vector2 startPosition = Vector2::Up * y * height;	// �n�_�̍��W
			Vector2 endPosition = Vector2::Up * preY * height;	// �I�_�̍��W
			Vector2 position = MATH::Leap(startPosition, endPosition, t);	// ���`�⊮
			DrawSpriteCenter(0, position, 0.0f, Vector2(600, height), Color::White);
		}
	}
}
