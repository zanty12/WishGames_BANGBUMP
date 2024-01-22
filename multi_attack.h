#pragma once
/***********************************************************
	Server
************************************************************/
class ServerAttack : public ServerGameObject {
private:
	ServerGameObject *self = nullptr;

public:
	int damage = 0;
	int drop = 0;



public:
	ServerAttack(int damage, int drop, float radius, ServerGameObject *self) : self(self), damage(damage), drop(drop) { this->radius = radius; }

	const ServerGameObject *GetSelf(void) { return self; }
	virtual MULTI_OBJECT_TYPE GetType(void) = 0;
};





/***********************************************************
	Client
************************************************************/
class ClientAttack : public ClientGameObject {
public:
	ClientAttack(Transform transform) : ClientGameObject(transform) { }

	virtual MULTI_OBJECT_TYPE GetType(void) = 0;
};