#include "multi_ui.h"

Vector2 CalcIconPosition(int id, int maxMembers, int width) {
	float centerX = Graphical::GetWidth() * 0.5f;	// ��ʂ̒����iX���W�j
	float center = (float)maxMembers * 0.5f - 0.5f;	// ���S��Idx���v�Z
	float x = center - id;							// X���W���v�Z
	x *= -1.0f;
	return Vector2(centerX + x * width, 100);
}

Vector2 CalcTimePosition(void) {
	float centerX = Graphical::GetWidth() * 0.5f;	// ��ʂ̒����iX���W�j
	return Vector2(centerX, Graphical::GetHeight() - 100.0f);
}