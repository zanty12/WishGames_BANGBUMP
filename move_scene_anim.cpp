#include "move_scene_anim.h"
#include "texture.h"
#include "sprite.h"

float MoveScene::rate = 0.0f;
float MoveScene::dstRate = 0.0f;
int MoveScene::sceneTexNo = 0;


void MoveScene::Initialize(void) {
	sceneTexNo = LoadTexture("data/texture/white.jpg");
}
void MoveScene::Loop(void) {
	float amountRate = 0.1f;
	float direction = dstRate - rate;
	float amount = direction * amountRate;
	if (MATH::Abs(direction) <= 0.05f) {
		rate = dstRate;
	}
	else {
		rate += amount;
	}


	float width = Graphical::GetWidth();
	float height = Graphical::GetHeight();
	Vector2 pos = Vector2(width, height) * 0.5f;
	Vector2 scl = Vector2(width, height);
	Color col = Color(1.0f, 1.0f, 1.0f, rate);
	DrawSprite(sceneTexNo, pos, 0.0f, scl, col);
}
bool MoveScene::Move(float dstRate) {
	MoveScene::dstRate = dstRate;

	float direction = MoveScene::dstRate - rate;
	if (MATH::Abs(direction) <= 0.05f) return true;
	else return false;
}
