#pragma once
#include "multi_transform.h"
#include "multi_behavior.h"


class ServerGameObject : public MultiBehavior {
public:
	Transform transform;
	Vector2 velocity;
	float radius = 10.0f;
	int texNo = -1;

	virtual void Update(void) { };
};