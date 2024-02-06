#include "multi_enemy.h"
#include "multiplay.h"

void EnemyServerSide::Damage(AttackServerSide *attack) {
	if (GetType() == MULTI_ENEMY1 && attack->GetType() == MULTI_ATTACK_ENEMY2 ||
		GetType() == MULTI_ENEMY2 && attack->GetType() == MULTI_ATTACK_ENEMY2 ||
		GetType() == MULTI_ENEMY3 && attack->GetType() == MULTI_ATTACK_ENEMY2) return;
	hp -= attack->atk;
	damageEffectAttributeType = attack->GetType();
	if (hp <= 0) {
		// �X�L���I�[�u���h���b�v������
		map->DropSkillOrb(deathDrop, transform.position, 10.0f);
		Destroy();

		// �U���҂��v���C���[�Ȃ�X�R�A�����Z����
		auto attackerType = attack->GetSelf()->GetType();
		if (attackerType == MULTI_PLAYER) {
			auto player = attack->GetSelf()->Cast<ServerPlayer>();
			player->score += score;
		}
	}
}
void EnemyServerSide::BlownPlayers(void) {
	// �G�l�~�[�ɑ΂���_���[�W����
	for (auto &kvp : MultiPlayServer::clients_) {
		auto &player = kvp.second.player_;

		float maxRadius = radius + player->radius;
		float maxRadiusSq = maxRadius * maxRadius;

		// �_���[�W
		if (maxRadiusSq >= Vector2::DistanceSq(transform.position, player->transform.position)) {
			// �h���b�v
			player->SkillOrbDrop(atkDrop);
			// ������΂�
			Vector2 direction = player->transform.position - transform.position;
			player->blownVelocity = direction.Normalize() * knockbackRate;
		}
	}
}






void Enemy1ServerSide::Loop(void) {
	// �_���[�W����������
	damageEffectAttributeType = -1;

	// �ړ�
	transform.position += velocity;
	if (map->Collision(transform.position, radius) != -1) {
		velocity *= -1.0f;
	}

	// �v���C���[�ɐG�ꂽ�琁����΂�
	BlownPlayers();
}
void Enemy1ClientSide::Loop(void) {
	if (!isShow) return;
	anim.Draw(transform.position - MultiPlayClient::offset, 0.0f, Vector2::One * radius, Color::White, velocity.x > 0.0f);
	isShow = false;
}




void Enemy2ServerSide::Loop(void) {
	// �_���[�W����������
	damageEffectAttributeType = -1;

	// �U������
	if (coolTime < spawnTimer.GetNowTime() * 0.001f) {
		float minDistanceSq = -1.0f;
		Vector2 targetPosition;
		float searchRadiusSq = activeRadius * activeRadius;

		// �ł��߂��v���C���[�𒲂ׂ�
		for (auto &client : MultiPlayServer::clients_) {
			auto &player = client.second.player_;
			// �����̌v�Z
			float distanceSq = Vector2::DistanceSq(transform.position, player->transform.position);
			float maxRadiusSq = radius + player->radius;
			maxRadiusSq = maxRadiusSq * maxRadiusSq;

			if (distanceSq <= searchRadiusSq) {
				if (minDistanceSq < 0.0f || distanceSq < minDistanceSq) {
					targetPosition = player->transform.position;
				}
			}
		}

		// ���������߂�
		if (targetPosition != Vector2::Zero) {
			auto bullet = map->GetAttacks()->Add<AttackEnemy2ServerSide>(this);
			bullet->velocity = (targetPosition - transform.position).Normalize() * 10.0f;
		}

		spawnTimer.Start();
	}

	// �v���C���[�ɐG�ꂽ�琁����΂�
	BlownPlayers();
}
void Enemy2ClientSide::Loop(void) {
	if (!isShow) return;
	anim.Draw(transform.position - MultiPlayClient::offset, 0.0f, Vector2::One * 170, Color::White);
	isShow = false;
}
void AttackEnemy2ServerSide::Loop(void) {	
	// �ړ�
	transform.position += velocity;

	if (MultiPlayClient::GetGameMode())
		if (MultiPlayClient::GetGameMode()->GetMap()->Collision(transform.position, radius) != -1) Destroy();
}
void AttackEnemy2ServerSide::KnockBack(ServerMovableGameObject *object) {
	object->blownVelocity = (object->transform.position - transform.position) * knockbackRate;
}
void AttackEnemy2ClientSide::Loop(void) {
	if (!isShow) return;
	anim.Draw(transform.position - MultiPlayClient::offset, 0.0f, Vector2::One * radius, Color::White);
	isShow = false;
}





void Enemy3ServerSide::Loop(void) {
	// �_���[�W����������
	damageEffectAttributeType = -1;

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
	else velocity = Vector2::Zero;

	// �ړ�
	transform.position += velocity;

	map->Collision(transform.position, radius, &velocity);
	// �v���C���[�ɐG�ꂽ�琁����΂�
	BlownPlayers();
}
void Enemy3ClientSide::Loop(void) {
	if (!isShow) return;
	anim.Draw(transform.position - MultiPlayClient::offset, 0.0f, Vector2::One * radius, Color::White, velocity.x > 0.0f);
	isShow = false;
}



