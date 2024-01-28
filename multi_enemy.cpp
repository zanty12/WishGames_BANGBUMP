#include "multi_enemy.h"
#include "multiplay.h"

void Enemy1ServerSide::Loop(void) {
	if (map->Collision(transform.position, radius)) {
		velocity *= -1.0f;
	}
}

void Enemy2ServerSide::Loop(void)
{
}

void Enemy3ServerSide::Loop(void) {
	ServerPlayer *targetPlayer = nullptr;
	float minDistanceSq = -1.0f;

	// �ŒZ�����̃v���C���[�𒲂ׂ�
	for (auto &client : MultiPlayServer::clients_) {
		auto &player = client.second.player_;

		// �v���C���[�Ƃ̋��������߂�
		float distanceSq = Vector2::DistanceSq(player->transform.position, transform.position);

		// ���m�\�Ȕ͈͂ɂ��邩�ǂ���
		if (distanceSq < activeRadius * activeRadius) {
			// �߂����ǂ������ׂ�
			if (minDistanceSq < 0.0f || distanceSq < minDistanceSq) {
				targetPlayer = player;
				minDistanceSq = distanceSq;
			}
		}
	}

	if (targetPlayer) {
		Vector2 velocity = targetPlayer->transform.position - transform.position;
		velocity = velocity.Normalize() * speed;
		this->velocity = velocity;
	}
}


