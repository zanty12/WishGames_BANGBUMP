#include "move_scene_anim.h"
#include "texture.h"
#include "sprite.h"

Color MoveScene::rate = Color(0.0f,0.0f, 0.0f, 0.0f);
Color MoveScene::dstRate = Color(0.0f, 0.0f, 0.0f, 0.0f);
int MoveScene::sceneTexNo = 0;


void MoveScene::Initialize(void) {
	sceneTexNo = LoadTexture("data/texture/white.jpg");
}
void MoveScene::Loop(void) {
	float amountRate = 0.1f;
	Color direction = dstRate - rate;
	Color amount = direction * amountRate;
	float distance = direction.r * direction.r + direction.g * direction.g + direction.b * direction.b + direction.a * direction.a;
	if (distance <= 0.05f) {
		rate = dstRate;
	}
	else {
		rate += amount;
	}


	float width = Graphical::GetWidth();
	float height = Graphical::GetHeight();
	Vector2 pos = Vector2(width, height) * 0.5f;
	Vector2 scl = Vector2(width, height);
	DrawSprite(sceneTexNo, pos, 0.0f, scl, rate);
}
bool MoveScene::Move(Color dstRate) {
	MoveScene::dstRate = dstRate;

	Color direction = MoveScene::dstRate - rate;
	float distance = direction.r * direction.r + direction.g * direction.g + direction.b * direction.b + direction.a * direction.a;
	if (distance <= 0.05f) return true;
	else return false;
}
