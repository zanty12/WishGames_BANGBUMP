#pragma once
#include "multi_object.h"

class AttackServerSide;
class ServerMovableGameObject : public GameObjectServerSide {
public:
	Vector2 blownVelocity;								// �_���[�W��������΂��ꂽ�x���V�e�B
	float friction = 0.95f;								// �����x
	float blownFriction = 0.85f;						// �_���[�W��������΂��ꂽ�ۂ̌����x
	Vector2 gravityVelocity;							// �d�̓x�N�g��

	int damageEffectAttributeType = -1;					// �_���[�W�^����ꂽ���̑����^�C�v
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
