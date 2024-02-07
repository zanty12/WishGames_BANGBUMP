#pragma once
#include "multi_attack.h"


/***********************************************************
	Server
************************************************************/
class SpikeServerSide : public AttackServerSide {
public:
	Vector2 blownVelocity;
	SpikeServerSide(Transform transform, Vector2 blownVelocity) : 
		blownVelocity(blownVelocity), AttackServerSide(0, 5, 1.0f, 2.0f, 25.0f, this) { this->transform.position = transform.position; }

	void KnockBack(ServerMovableGameObject *object) override;
	MULTI_OBJECT_TYPE GetType(void) { return MULTI_OBJECT_TYPE::MULTI_SPIKE; }
};





/***********************************************************
	Client
************************************************************/
class ClientServerSide : public AttackClientSide {

};
