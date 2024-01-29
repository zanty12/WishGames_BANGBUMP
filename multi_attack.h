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
	PHYSICS::Vertex4 collider;	// コライダー処理



public:
	AttackServerSide(int atk, int atkDrop, float radius, GameObjectServerSide *self) : self(self), atk(atk), atkDrop(atkDrop) { this->radius = radius; }

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