#include "multi_object.h"

int GameObjectServerSide::MAX_ID = 0;

bool GameObjectServerSide::CircleTouch(GameObjectServerSide *object) {
	float maxRadius = object->radius + radius;									// �ő唼�a
	//float maxRadiusSq = object->radius * object->radius + radius * radius;		// �ő唼�a�i�����j
	float maxRadiusSq = maxRadius * maxRadius;									// �ő唼�a�i�����j

	// �~�̔���
	Vector2 direction = object->transform.position - transform.position;	// �I�u�W�F�N�g�̌���
	float distanceSq = direction.DistanceSq();
	if (distanceSq <= maxRadiusSq) {
		int i = 0;
	}
	return distanceSq <= maxRadiusSq;
}
