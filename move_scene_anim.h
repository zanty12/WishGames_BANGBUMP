#pragma once
#include "lib/color.h"
#include "multi_anim.h"

class MoveScene {
private:
	Color rate = Color(0.0f, 0.0f, 0.0f, 0.0f);
	Color dstRate = Color(0.0f, 0.0f, 0.0f, 0.0f);
	static int sceneTexNo;
	static int loadingTexNo;
	static bool isLoadingShow;
	MultiAnimator loadingAnim;

public:
	void Initialize(void);
	void Loop(void);
	bool Move(Color dstRate, bool isLoadingShow = false);
};


extern MoveScene AllMoveScene;
extern MoveScene UIMoveScene;
