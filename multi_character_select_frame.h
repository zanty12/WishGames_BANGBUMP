#pragma once
#include "follow.h"
#include "sprite.h"
#include "multi_anim.h"

class CharacterSelectFrameClientSide {
public:
	int id = 0;
	int maxNum = 4;
	int state = 0;
	int prevState = 0;
	Follow stateSmooth = 0.0f;
	Follow moveSmooth = 0.0f;
	Follow attackSmooth = 0.0f;
	MultiAnimator swordAnim;
	CharacterSelectFrameClientSide() {
		swordAnim.width = 5;
		swordAnim.height = 3;
		swordAnim.begin = 0;
		swordAnim.end = 14;
		swordAnim.SetFrame(1000 / 30);
		swordAnim.isLoop = false;
		swordAnim.isEndShow = true;
		swordAnim.MoveEnd();
	}

	void Draw(int frameTexNo, int framePTexNo, int arrowTexNo, int playerTexNo, int bootTexNo, int handTexNo, int swordTexNo, bool isShow, float width, float height, float gap, int moveAttribute, int attackAttribute, int character_select_so);
};