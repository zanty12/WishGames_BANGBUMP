#pragma once
#include "lib/color.h"

class MoveScene {
private:
	static Color rate;
	static Color dstRate;
	static int sceneTexNo;

public:
	static void Initialize(void);
	static void Loop(void);
	static bool Move(Color dstRate);
};