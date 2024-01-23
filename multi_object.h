#pragma once
#include "multi_transform.h"
#include "multi_behavior.h"
#include "multi_objenum.h"

class MultiMap;
class GameObjectServerSide : public MultiBehavior {
public:
	static int MAX_ID;

	Transform transform;
	Vector2 velocity;
	float radius = 10.0f;
	int id = MAX_ID++;

	GameObjectServerSide() = default;
	GameObjectServerSide(Transform transform) : transform(transform) { }
	virtual MULTI_OBJECT_TYPE GetType(void) = 0;
};

class ClientGameObject : public MultiBehavior {
public:
	Transform transform;
	Vector2 velocity;
	float radius = 10.0f;
	int texNo = -1;
	int id = 0;
	bool isShow = true;


	ClientGameObject() = default;
	ClientGameObject(Transform transform) : transform(transform) { };
};