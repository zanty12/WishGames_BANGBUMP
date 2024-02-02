#pragma once
#include "multi_object.h"

class AttackServerSide;
class ServerMovableGameObject : public GameObjectServerSide {
public:
	Vector2 blownVelocity;								// ダメージ時吹き飛ばされたベロシティ
	float friction = 0.95f;								// 減速度
	float blownFriction = 0.85f;						// ダメージ時吹き飛ばされた際の減速度
	Vector2 gravityVelocity;							// 重力ベクトル

	int damageEffectAttributeType = -1;					// ダメージ与えられた時の属性タイプ
	float gravity = 0.0f;
	float maxGravity = 0.5f;



public:
	ServerMovableGameObject() = default;
	ServerMovableGameObject(Transform transform) : GameObjectServerSide(transform) { }
	virtual void Damage(AttackServerSide *attack) { };
};




class AttackClientSide;
class ClientMovableGameObject : public GameObjectClientSide {
public:
	ClientMovableGameObject() = default;
	ClientMovableGameObject(Transform transform) : GameObjectClientSide(transform) { };
};
