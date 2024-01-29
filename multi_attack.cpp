#include "multi_attack.h"
#include "lib/math.h"

bool AttackServerSide::Touch(GameObjectServerSide *object) {
	Vector2 seg = this->direction;											// �����̌���
	Vector2 segNor = seg.Normalize();										// �����̒P�ʃx�N�g��
	float segDistanceSq = seg.DistanceSq();									// �����̒����i�����j
	float maxRadius = object->radius + radius;								// �ő唼�a
	float maxRadiusSq = maxRadius * maxRadius;								// �ő唼�a�i�����j

	Vector2 direction = object->transform.position - transform.position;	// �I�u�W�F�N�g�̌���
	float horizontal = Vector2::Dot(segNor, direction);						// ���������ɂ���X���W
	float horizontalSq = horizontal * horizontal;							// �I�u�W�F�N�g�̊O�ρi�����j

	// �����͈͓̔�
	if (0.0f < horizontalSq && horizontalSq < segDistanceSq) {
		float vertical = Vector2::Cross(segNor, direction);
		return MATH::Abs(vertical) <= maxRadius;
	}
	// �n�_
	else if (horizontalSq < 0.0f) {
		return direction.DistanceSq() <= maxRadiusSq;
	}
	// �I�_
	else {
		Vector2 directionFromEnd = object->transform.position + (transform.position + this->direction);
		return directionFromEnd.DistanceSq() <= maxRadiusSq;
	}
}