#pragma once
#include "multi_anim.h"
#include "multi_object.h"

class ResultSkillOrb : public GameObjectClientSide {
public:
	Vector2 velocity;
	Vector2 target;
	float moveAmoument = 0.25f;
	float destroyDistance = 5.0f;
	bool isDestroy = false;
	MultiAnimator anim;

	ResultSkillOrb(Vector2 target, Vector2 velocity) : target(target), velocity(velocity) {
		//anim = MultiAnimator()
	}

	void Loop(void) override;

	bool IsDestroy(void) { return isDestroy; }
};