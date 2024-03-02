#pragma once
#include "multi_anim.h"

class RankingAnimator {
	float t = 0.0f;
public:
	MultiAnimator anim;

	RankingAnimator() = default;
	RankingAnimator(MultiAnimator anim) : anim(anim) {};
	void Draw(Vector2 position, Vector2 scale, Color color);
	void Draw(Vector2 position, Vector2 scale, Vector2 pos1, Vector2 pos2, Vector2 pos3, Vector2 pos4, Color col1, Color col2, Color col3, Color col4, float addRate);
};