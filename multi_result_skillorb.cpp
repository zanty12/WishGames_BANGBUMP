#include "multi_result_skillorb.h"

void ResultSkillOrb::Loop(void) {
	// ターゲットの向きを計算
	Vector2 direction = target - transform.position;

	// ベロシティ
	velocity += direction * moveAmoument;

	// ターゲットないなら削除する
	if (direction.DistanceSq() <= destroyDistance) {
		isDestroy = true;
	}

	// 移動
	transform.position += velocity;

	anim.Draw(transform.position, transform.rotation, transform.scale, Color::White);
}
