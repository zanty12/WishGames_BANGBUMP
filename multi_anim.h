#pragma once
#include "lib/vector.h"

class MultiAnim {
private:
	int idx = 0;

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
	MultiAnim(int texNo)
		: texNo(texNo), width(1), height(1), begin(0), end(0), isLoop(false), loopBegin(0), loopEnd(0) { };
	MultiAnim(int texNo, int width, int height, int begin, int end)
		: texNo(texNo), width(width), height(height), begin(begin), end(end), isLoop(false), loopBegin(0), loopEnd(0) { };
	MultiAnim(int texNo, int width, int height, int begin, int end, bool isLoop, int loopBegin, int loopEnd)
		: texNo(texNo), width(width), height(height), begin(begin), end(end), isLoop(isLoop), loopBegin(loopBegin), loopEnd(loopEnd) { };

	int ToIndex(int x, int y) { return x + y * width; }
	Vector2 ToUV(int idx) { return Vector2((idx % width) / (float)width, (idx / height) / (float)height); }
	Vector2 ToUV(int x, int y) { return Vector2(x / (float)width, y / (float)height); }

	void Loop(void) {

	}
};