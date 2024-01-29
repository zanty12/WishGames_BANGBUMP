#include "multi_attack.h"
#include "lib/math.h"

bool AttackServerSide::Touch(GameObjectServerSide *object) {
	Vector2 seg = this->direction;											// 線分の向き
	Vector2 segNor = seg.Normalize();										// 線分の単位ベクトル
	float segDistanceSq = seg.DistanceSq();									// 線分の長さ（平方）
	float maxRadius = object->radius + radius;								// 最大半径
	float maxRadiusSq = maxRadius * maxRadius;								// 最大半径（平方）

	Vector2 direction = object->transform.position - transform.position;	// オブジェクトの向き
	float horizontal = Vector2::Dot(segNor, direction);						// 線分を軸にしたX座標
	float horizontalSq = horizontal * horizontal;							// オブジェクトの外積（平方）

	// 線分の範囲内
	if (0.0f < horizontalSq && horizontalSq < segDistanceSq) {
		float vertical = Vector2::Cross(segNor, direction);
		return MATH::Abs(vertical) <= maxRadius;
	}
	// 始点
	else if (horizontalSq < 0.0f) {
		return direction.DistanceSq() <= maxRadiusSq;
	}
	// 終点
	else {
		Vector2 directionFromEnd = object->transform.position + (transform.position + this->direction);
		return directionFromEnd.DistanceSq() <= maxRadiusSq;
	}
}