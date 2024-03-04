#include "multi_attack.h"
#include "lib/collider2d.h"
#include "lib/math.h"
#include "sprite.h"
#include "multiplay.h"

bool AttackServerSide::Touch(GameObjectServerSide *object, Vector2 localPosition) {
	if (self == object) return false;


	float maxRadius = object->radius + radius;									// �ő唼�a
	//float maxRadiusSq = object->radius * object->radius + radius * radius;		// �ő唼�a�i�����j
	float maxRadiusSq = maxRadius * maxRadius;									// �ő唼�a�i�����j

	// �~�̔���
	if (this->direction == Vector2::Zero) {
		Vector2 direction = object->transform.position - transform.position;	// �I�u�W�F�N�g�̌���
		float distanceSq = direction.DistanceSq();
		if (distanceSq <= maxRadiusSq) {
			int i = 0;
		}
		return distanceSq <= maxRadiusSq;
	}
	// �J�v�Z������
	else {
		Vector2 seg = this->direction;												// �����̌���
		Vector2 segNor = seg.Normalize();											// �����̒P�ʃx�N�g��
		float segDistance = seg.Distance();											// �����̒���

		Vector2 direction = object->transform.position + localPosition - transform.position;		// �I�u�W�F�N�g�̌���
		float horizontal = Vector2::Dot(segNor, direction);							// ���������ɂ���X���W

		// �����͈͓̔�
		if (0.0f <= horizontal && horizontal < segDistance) {
			float vertical = Vector2::Cross(segNor, direction);
			return MATH::Abs(vertical) <= maxRadius;
		}
		// �n�_
		else if (horizontal < 0.0f) {
			return direction.Distance() <= maxRadius;
		}
		// �I�_
		else {
			Vector2 directionFromEnd = object->transform.position + localPosition - (transform.position + this->direction);
			return directionFromEnd.Distance() <= maxRadius;
		}
	}
}

