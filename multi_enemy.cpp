#include "multi_enemy.h"
#include "multiplay.h"

void EnemyServerSide::BlownPlayers(void) {
	// エネミーに対するダメージ処理
	for (auto &kvp : MultiPlayServer::clients_) {
		auto &player = kvp.second.player_;

		float maxRadius = radius + player->radius;
		float maxRadiusSq = maxRadius * maxRadius;

		// ダメージ
		if (maxRadiusSq >= Vector2::DistanceSq(transform.position, player->transform.position)) {
			player->SkillOrbDrop(atkDrop);
			// 吹き飛ばす
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

	// プレイヤーに触れたら吹き飛ばす
	BlownPlayers();
}


void Enemy2ServerSide::Loop(void) {
	// プレイヤーに触れたら吹き飛ばす
	BlownPlayers();
}

void Enemy3ServerSide::Loop(void) {
	ServerPlayer *targetPlayer = nullptr;
	float minDistanceSq = -1.0f;

	// 最短距離のプレイヤーを調べる
	for (auto &client : MultiPlayServer::clients_) {
		auto &player = client.second.player_;

		// プレイヤーとの距離を求める
		float distanceSq = Vector2::DistanceSq(player->transform.position, transform.position);

		// 検知可能な範囲にいるかどうか
		if (distanceSq < activeRadius * activeRadius) {
			// 近いかどうか調べる
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

	// プレイヤーに触れたら吹き飛ばす
	BlownPlayers();
}

void Enemy1ClientSide::Loop(void) {
	if (!isShow) return;
	anim.Draw(transform.position - MultiPlayClient::offset, 0.0f, Vector2::One * 100, Color::White, velocity.x > 0.0f);
	isShow = true;
}


