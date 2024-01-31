#pragma once
#include <list>
#include "lib/vector.h"
#include "lib/win_time.h"
#include "texture.h"

class LightEffect {
private:
	struct Light {
		float rad = 0.0f;
		float radSpeed = MATH::Rand(-0.01f, 0.01f);
		Vector2 position;
		Vector2 scale = Vector2(100.0f, 100.0f);
		WIN::Time timer;
	};

private:
	int texNo;
	std::list<Light> lights;
	float spawnTime = 2.0f;
	float lifeTime = 5.0f;
	WIN::Time timer;

public:
	LightEffect() {
		texNo = LoadTexture("data/texture/Light/light_effect_01.png");
		timer.Start();
	}

	void Draw(Vector2 offset);
};