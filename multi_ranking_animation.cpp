#include "multi_ranking_animation.h"
#include "lib/math.h"

void RankingAnimator::Draw(Vector2 position, Vector2 scale, Color color) {
	anim.Draw(position, 0.0f, scale, color);
}

void RankingAnimator::Draw(Vector2 position, Vector2 scale, Vector2 pos1, Vector2 pos2, Vector2 pos3, Vector2 pos4, Color col1, Color col2, Color col3, Color col4, float addRate) {
	t += addRate;
	if (1.0f < t) {
		t = 1.0f;
		anim.idx = anim.end;
	}

	Vector2 localPosition = MATH::Bezier(pos1, pos2, pos3, pos4, t);
	Color color = MATH::Bezier(col1, col2, col3, col4, t);
	anim.idx = anim.end * t;

	anim.Draw(position + localPosition, 0.0f, scale, color);
}
