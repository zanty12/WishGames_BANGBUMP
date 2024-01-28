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
	int damage = 0;
	int drop = 0;
	float spanTime = 1.0f;
	std::map<GameObjectServerSide*, WIN::Time> touchGameObjects;



public:
	AttackServerSide(int damage, int drop, float radius, GameObjectServerSide *self) : self(self), damage(damage), drop(drop) { this->radius = radius; }

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