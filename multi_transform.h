#pragma once
#include "lib/vector.h"

struct Transform {
	Vector2 position = Vector2::Zero;
	float rotation = 0.0f;
	Vector2 scale = Vector2::One;

	Transform() { }
	Transform(Vector2 pos) : position(pos) { }
	Transform(Vector2 pos, float rot) : position(pos), rotation(rot) { }
	Transform(Vector2 pos, float rot, Vector2 scl) : position(pos), rotation(rot), scale(scl) { }
};