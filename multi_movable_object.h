#pragma once
#include "multi_object.h"

class AttackServerSide;
class ServerMovableGameObject : public GameObjectServerSide {
public:
	Vector2 blownVelocity;								// ダメージ時吹き飛ばされたベロシティ
	float blownFriction = 0.85f;						// ダメージ時吹き飛ばされた際の減速度



public:
	ServerMovableGameObject() = default;
	ServerMovableGameObject(Transform transform) : GameObjectServerSide(transform) { }
	virtual void Damage(AttackServerSide *attack) { };
};




class ClientAttack;
class ClientMovableGameObject : public GameObjectClientSide {
public:
	ClientMovableGameObject() = default;
	ClientMovableGameObject(Transform transform) : GameObjectClientSide(transform) { };
};
