#pragma once
#include "multi_attribute.h"
#include "multi_object.h"

class ServerAttack;
class ServerMovableGameObject : public ServerGameObject {
public:
	ServerMovableGameObject() = default;
	ServerMovableGameObject(Transform transform) : ServerGameObject(transform) { }
	virtual void Damage(ServerAttack *attack) { };
};




class ClientAttack;
class ClientMovableGameObject : public ClientGameObject {
public:
	ClientMovableGameObject() = default;
	ClientMovableGameObject(Transform transform) : ClientGameObject(transform) { };
};
