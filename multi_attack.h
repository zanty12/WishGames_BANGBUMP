#pragma once
#include <map>
#include "lib/win_time.h"
#include "multi_object.h"

class ServerMovableGameObject;
/***********************************************************
	Server
************************************************************/
class AttackServerSide : public GameObjectServerSide {
protected:
	GameObjectServerSide *self = nullptr;

public:
	int atk = 0;
	int atkDrop = 0;
	float spanTime = 1.0f;
	float knockbackRate = 10.0f;
	std::map<GameObjectServerSide*, WIN::Time> touchGameObjects;
	Vector2 direction;



public:
	AttackServerSide(int atk, int atkDrop, float spanTime, float knockbackRate, float radius, GameObjectServerSide *self) : self(self), atk(atk), atkDrop(atkDrop), spanTime(spanTime), knockbackRate(knockbackRate) { this->radius = radius; }
	bool Touch(GameObjectServerSide *object);

	const GameObjectServerSide *GetSelf(void) { return self; }
	virtual MULTI_OBJECT_TYPE GetType(void) = 0;
	virtual void KnockBack(ServerMovableGameObject *object) {};
};





/***********************************************************
	Client
************************************************************/
class ClientAttack : public GameObjectClientSide {
public:
	ClientAttack(Transform transform) : GameObjectClientSide(transform) { }

	virtual MULTI_OBJECT_TYPE GetType(void) = 0;
};