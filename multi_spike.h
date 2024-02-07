#pragma once
#include "multi_attack.h"
#include "multi_map.h"
#include <iostream>


/***********************************************************
	Server
************************************************************/
class SpikeServerSide : public AttackServerSide {
public:
	Vector2 blownVelocity;
	SpikeServerSide(Transform transform, Vector2 blownVelocity) : 
		blownVelocity(blownVelocity), AttackServerSide(0, 5, 1.0f, 10.0f, MultiMap::cellSize * 0.5f, this) { 
		this->transform.position = transform.position;
	}

	void KnockBack(ServerMovableGameObject *object) override;
	MULTI_OBJECT_TYPE GetType(void) { return MULTI_OBJECT_TYPE::MULTI_SPIKE; }
};





/***********************************************************
	Client
************************************************************/
class ClientServerSide : public AttackClientSide {

};
