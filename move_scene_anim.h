#pragma once
#include "lib/color.h"
#include "multi_anim.h"

class MoveScene {
private:
	static Color rate;
	static Color dstRate;
	static int sceneTexNo;
	static int loadingTexNo;
	static bool isLoadingShow;
	static MultiAnimator loadingAnim;

public:
	static void Initialize(void);
	static void Loop(void);
	static bool Move(Color dstRate, bool isLoadingShow = false);
};
