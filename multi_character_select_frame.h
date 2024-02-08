#pragma once
#include "follow.h"
#include "sprite.h"

class CharacterSelectFrameClientSide {
public:
	int id = 0;
	int maxNum = 4;
	Follow stateSmooth = 0.0f;
	Follow moveSmooth = 0.0f;
	Follow attackSmooth = 0.0f;

	void Draw(int frameTexNo, int framePTexNo, int playerTexNo, int bootTexNo, int handTexNo, bool isShow, float width, float height, float gap, int moveAttribute, int attackAttribute);
};