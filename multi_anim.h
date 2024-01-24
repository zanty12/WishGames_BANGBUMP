#pragma once
#include "time.h"
#include "multi_animenum.h"
#include "attribute_type.h"
#include "sprite.h"

class MultiAnimator {
private:
	int idx = 0;
	DWORD startTime = 0;
	DWORD frameTime = 1000 / 30;

public:
	int texNo = 0;
	int width = 1;
	int height = 1;
	bool isLoop = true;
	int begin = 0;
	int end = 1;
	int loopBegin = 0;
	int loopEnd = 1;



public:
	MultiAnimator() = default;
	MultiAnimator(int texNo)
		: texNo(texNo), width(1), height(1), begin(0), end(0), isLoop(false), loopBegin(0), loopEnd(0), startTime(timeGetTime()), idx(0) { };
	MultiAnimator(int texNo, int width, int height, int begin, int end)
		: texNo(texNo), width(width), height(height), begin(begin), end(end), isLoop(false), loopBegin(0), loopEnd(0), startTime(timeGetTime()), idx(begin) { };
	MultiAnimator(int texNo, int width, int height, int begin, int end, bool isLoop, int loopBegin, int loopEnd)
		: texNo(texNo), width(width), height(height), begin(begin), end(end), isLoop(isLoop), loopBegin(loopBegin), loopEnd(loopEnd), startTime(timeGetTime()), idx(begin) { };

	int ToIndex(int x, int y) { return x + y * width; }
	Vector2 ToUV(int idx) { return Vector2((idx % width) / (float)width, (idx / width) / (float)height); }
	Vector2 ToUV(int x, int y) { return Vector2(x / (float)width, y / (float)height); }

	void Draw(Vector2 pos, float rot, Vector2 scl, Color col, bool isReverseX = false, bool isReverseY = false);

	void MoveBegin(void) { idx = begin; }
	void MoveEnd(void) { idx = end + 1; }
	void MoveLoopBegin(void) { idx = loopBegin; }
	void MoveLoopEnd(void) { idx = loopEnd + 1; }
	bool IsEnd(void) { return idx + 1 <= end; }

	// プレイヤーのアニメーションデータ
	static MultiAnimator GetPlayerInitialize(int playerIdx, ATTRIBUTE_TYPE move, ATTRIBUTE_TYPE attack);
	// プレイヤーのアニメーションデータ
	static void GetPlayer(MULTI_ANIMATION_TYPE animType, ATTRIBUTE_TYPE move, ATTRIBUTE_TYPE attack, MultiAnimator* anim);
};