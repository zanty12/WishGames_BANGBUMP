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
		Vector2 seg = this->direction;											// 線分の向き
		Vector2 segNor = seg.Normalize();										// 線分の単位ベクトル
		float segDistanceSq = seg.DistanceSq();									// 線分の長さ（平方）
		float maxRadius = object->radius + radius;								// 最大半径

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
			return direction.DistanceSq() <= maxRadius;
		}
		// 終点
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