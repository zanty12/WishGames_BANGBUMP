#include "multi_attack.h"
#include "lib/collider2d.h"
#include "lib/math.h"
#include "sprite.h"
#include "multiplay.h"

bool AttackServerSide::Touch(GameObjectServerSide *object, Vector2 localPosition) {
	float maxRadius = object->radius + radius;									// �ő唼�a
	float maxRadiusSq = maxRadius * maxRadius;									// �ő唼�a�i�����j

	// �~�̔���
	if (this->direction == Vector2::Zero) {
		Vector2 direction = object->transform.position + localPosition - transform.position;	// �I�u�W�F�N�g�̌���
		float distanceSq = direction.DistanceSq();
		return distanceSq <= maxRadiusSq;
	}
	// �J�v�Z������
	else {
		Vector2 seg = this->direction;												// �����̌���
		Vector2 segNor = seg.Normalize();											// �����̒P�ʃx�N�g��
		float segDistanceSq = seg.DistanceSq();										// �����̒����i�����j

		Vector2 direction = object->transform.position + localPosition - transform.position;		// �I�u�W�F�N�g�̌���
		float horizontal = Vector2::Dot(segNor, direction);							// ���������ɂ���X���W
		float horizontalSq = horizontal * horizontal;								// �I�u�W�F�N�g�̊O�ρi�����j

		// �����͈͓̔�
		if (0.0f <= horizontal && 0.0f < horizontalSq && horizontalSq < segDistanceSq) {
			float vertical = Vector2::Cross(segNor, direction);
			return MATH::Abs(vertical) <= maxRadius;
		}
		// �n�_
		else if (horizontalSq < 0.0f) {
			return direction.DistanceSq() <= maxRadiusSq;
		}
		// �I�_
		else {
			Vector2 directionFromEnd = object->transform.position + localPosition - (transform.position + this->direction);
			return directionFromEnd.DistanceSq() <= maxRadiusSq;
		}
	}
}

