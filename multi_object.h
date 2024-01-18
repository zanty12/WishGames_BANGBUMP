#pragma once
#include "multi_transform.h"
#include "multi_behavior.h"


class ServerGameObject : public MultiBehavior {
public:
	static int MAX_ID;

	Transform transform;
	Vector2 velocity;
	float radius = 10.0f;
	int id = MAX_ID++;


	ServerGameObject() = default;
	ServerGameObject(Transform transform) : transform(transform) { }
	virtual void Update(void) { };
};