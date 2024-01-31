#pragma once
#include "texture.h"
#include "sprite.h"

class Number {
private:
	static int texNo;

public:
	Number(Vector2 pos, Vector2 scl, int value, int direction = 0);
};