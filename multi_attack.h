#pragma once
#include <map>
#include "lib/win_time.h"

/***********************************************************
	Server
************************************************************/
class AttackServerSide : public GameObjectServerSide {
private:
	GameObjectServerSide *self = nullptr;

public:
	int atk = 0;
	int atkDrop = 0;
	float spanTime = 1.0f;
	std::map<GameObjectServerSide*, WIN::Time> touchGameObjects;
	Vector2 direction;



public:
	AttackServerSide(int atk, int atkDrop, float radius, GameObjectServerSide *self) : self(self), atk(atk), atkDrop(atkDrop) { this->radius = radius; }
	bool Touch(GameObjectServerSide *object) {
		Vector2 seg = this->direction;											// �����̌���
		Vector2 segNor = seg.Normalize();										// �����̒P�ʃx�N�g��
		float segDistanceSq = seg.DistanceSq();									// �����̒����i�����j
		float maxRadius = object->radius + radius;								// �ő唼�a

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
			return direction.DistanceSq() <= maxRadius;
		}
		// �I�_
		else {
			return (direction - this->direction).DistanceSq() <= maxRadius;
		}
	}

	const GameObjectServerSide *GetSelf(void) { return self; }
	virtual MULTI_OBJECT_TYPE GetType(void) = 0;
};





/***********************************************************
	Client
************************************************************/
class ClientAttack : public GameObjectClientSide {
public:
	ClientAttack(Transform transform) : GameObjectClientSide(transform) { }

	virtual MULTI_OBJECT_TYPE GetType(void) = 0;
};