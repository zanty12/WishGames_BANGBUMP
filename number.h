#pragma once
#include "texture.h"
#include "sprite.h"

class Number {
private:
	static int texNo;

public:
	Number(Vector2 pos, Vector2 scl, int value) {
		if (texNo == -1) {
			texNo = LoadTexture("data/texture/UI/number.png");
		}

		int digit = std::log10(value);
		if (digit == 0) digit = 1;
		int tmp = value;

		for (int i = 0; i < digit; i++) {
			int ones_place = value % 10;
			float v = ones_place * 0.1f;
			value *= 0.1f;

			DrawSpriteLeftTop(texNo, pos + Vector2::Left * scl.x * i, 0.0f, scl, Color::White, Vector2(0.0f, v), Vector2(1.0f, 0.1f));
		}
	}
};