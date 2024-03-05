#include "multi_object.h"

int GameObjectServerSide::MAX_ID = 0;

bool GameObjectServerSide::CircleTouch(GameObjectServerSide *object) {
	float maxRadius = object->radius + radius;									// 最大半径
	//float maxRadiusSq = object->radius * object->radius + radius * radius;		// 最大半径（平方）
	float maxRadiusSq = maxRadius * maxRadius;									// 最大半径（平方）

	// 円の判定
	Vector2 direction = object->transform.position - transform.position;	// オブジェクトの向き
	float distanceSq = direction.DistanceSq();
	if (distanceSq <= maxRadiusSq) {
		int i = 0;
	}
	return distanceSq <= maxRadiusSq;
}
