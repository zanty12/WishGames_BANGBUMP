#include "number.h"

int Number::texNo = -1;

Number::Number(Vector2 pos, Vector2 scl, int value, int direction) {
	if (texNo == -1) {
		texNo = LoadTexture("data/texture/UI/number.png");
	}

	int digit = (int)std::log10(value) + 1;
	float halfScale = scl.x * 0.5f;

	for (int i = 0; i < digit; i++) {
		int ones_place = value % 10;
		float v = ones_place * 0.1f;
		value *= 0.1f;

		float x = 0.0f;
		// •\Ž¦ˆÊ’u‚ðŒˆ‚ß‚é
		// ’†‰›
		if (direction == 0) {
			float center = (float)digit * 0.5f - 0.5f;
			x = center - i;
			x *= -1.0f;
		}
		else if (0 < direction) {
			x = i;
		}
		else {
			x = i - digit + 0.5f;
		}

		DrawSpriteLeftTop(texNo, pos + Vector2::Left * halfScale * x, 0.0f, scl, Color::White, Vector2(0.0f, v), Vector2(1.0f, 0.1f));
	}
}