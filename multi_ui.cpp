#include "multi_ui.h"

Vector2 CalcIconPosition(int id, int maxMembers, int width) {
	float centerX = Graphical::GetWidth() * 0.5f;	// 画面の中央（X座標）
	float center = (float)maxMembers * 0.5f - 0.5f;	// 中心のIdxを計算
	float x = center - id;							// X座標を計算
	x *= -1.0f;
	return Vector2(centerX + x * width, 100);
}

Vector2 CalcTimePosition(void) {
	float centerX = Graphical::GetWidth() * 0.5f;	// 画面の中央（X座標）
	return Vector2(centerX, Graphical::GetHeight() - 100.0f);
}