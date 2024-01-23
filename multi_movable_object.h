#pragma once
#include "multi_attribute.h"
#include "multi_object.h"

class AttackServerSide;
class ServerMovableGameObject : public GameObjectServerSide {
public:
	ServerMovableGameObject() = default;
	ServerMovableGameObject(Transform transform) : GameObjectServerSide(transform) { }
	virtual void Damage(AttackServerSide *attack) { };
};




class ClientAttack;
class ClientMovableGameObject : public ClientGameObject {
public:
	ClientMovableGameObject() = default;
	ClientMovableGameObject(Transform transform) : ClientGameObject(transform) { };
};
