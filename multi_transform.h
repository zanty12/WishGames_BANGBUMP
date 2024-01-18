#pragma once
#include "lib/vector.h"

struct Transform {
	Vector2 position = Vector2::Zero;
	float rotation = 0.0f;
	Vector2 scale = Vector2::One;

	Transform() = default;
	Transform(Vector2 pos, float rot = 0.0f, Vector2 scl = Vector2::One) : position(pos), rotation(rot), scale(scl) { }
};