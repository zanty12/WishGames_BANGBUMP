#pragma once

class MoveScene {
private:
	static float rate;
	static float dstRate;
	static int sceneTexNo;

public:
	static void Initialize(void);
	static void Loop(void);
	static bool Move(float dstRate);
};