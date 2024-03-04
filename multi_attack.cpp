#include "multi_attack.h"
#include "lib/collider2d.h"
#include "lib/math.h"
#include "sprite.h"
#include "multiplay.h"

bool AttackServerSide::Touch(GameObjectServerSide *object, Vector2 localPosition) {
	if (self == object) return false;


	float maxRadius = object->radius + radius;									// 最大半径
	//float maxRadiusSq = object->radius * object->radius + radius * radius;		// 最大半径（平方）
	float maxRadiusSq = maxRadius * maxRadius;									// 最大半径（平方）

	// 円の判定
	if (this->direction == Vector2::Zero) {
		Vector2 direction = object->transform.position - transform.position;	// オブジェクトの向き
		float distanceSq = direction.DistanceSq();
		if (distanceSq <= maxRadiusSq) {
			int i = 0;
		}
		return distanceSq <= maxRadiusSq;
	}
	// カプセル判定
	else {
		Vector2 seg = this->direction;												// 線分の向き
		Vector2 segNor = seg.Normalize();											// 線分の単位ベクトル
		float segDistance = seg.Distance();											// 線分の長さ

		Vector2 direction = object->transform.position + localPosition - transform.position;		// オブジェクトの向き
		float horizontal = Vector2::Dot(segNor, direction);							// 線分を軸にしたX座標

		// 線分の範囲内
		if (0.0f <= horizontal && horizontal < segDistance) {
			float vertical = Vector2::Cross(segNor, direction);
			return MATH::Abs(vertical) <= maxRadius;
		}
		// 始点
		else if (horizontal < 0.0f) {
			return direction.Distance() <= maxRadius;
		}
		// 終点
		else {
			Vector2 directionFromEnd = object->transform.position + localPosition - (transform.position + this->direction);
			return directionFromEnd.Distance() <= maxRadius;
		}
	}
}

