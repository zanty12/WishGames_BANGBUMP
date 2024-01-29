#include "multi_enemy.h"
#include "multiplay.h"

void EnemyServerSide::BlownPlayers(void) {
	// �G�l�~�[�ɑ΂���_���[�W����
	for (auto &kvp : MultiPlayServer::clients_) {
		auto &player = kvp.second.player_;

		float maxRadius = radius + player->radius;
		float maxRadiusSq = maxRadius * maxRadius;

		// �_���[�W
		if (maxRadiusSq >= Vector2::DistanceSq(transform.position, player->transform.position)) {
			player->SkillOrbDrop(atkDrop);
			// ������΂�
			Vector2 direction = player->transform.position - transform.position;
			player->blownVelocity = direction.Normalize() * 30.0f;
		}
	}
}






void Enemy1ServerSide::Loop(void) {
	transform.position += velocity;
	if (map->Collision(transform.position, radius) != -1) {
		velocity *= -1.0f;
	}

	// �v���C���[�ɐG�ꂽ�琁����΂�
	BlownPlayers();
}


void Enemy2ServerSide::Loop(void) {
	// �v���C���[�ɐG�ꂽ�琁����΂�
	BlownPlayers();
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

	// �v���C���[�ɐG�ꂽ�琁����΂�
	BlownPlayers();
}

void Enemy1ClientSide::Loop(void) {
	if (!isShow) return;
	anim.Draw(transform.position - MultiPlayClient::offset, 0.0f, Vector2::One * 100, Color::White, velocity.x > 0.0f);
	isShow = true;
}


