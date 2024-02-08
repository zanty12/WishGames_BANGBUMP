#include "move_scene_anim.h"
#include "texture.h"
#include "sprite.h"

Color MoveScene::rate = Color(0.0f,0.0f, 0.0f, 0.0f);
Color MoveScene::dstRate = Color(0.0f, 0.0f, 0.0f, 0.0f);
int MoveScene::sceneTexNo = -1;
int MoveScene::loadingTexNo = -1;
bool MoveScene::isLoadingShow = false;
MultiAnimator MoveScene::loadingAnim;




void MoveScene::Initialize(void) {
	sceneTexNo = LoadTexture("data/texture/white.jpg");
	loadingTexNo = LoadTexture("data/texture/UI/UI_NOWLOADLING.png");
	loadingAnim = MultiAnimator(loadingTexNo, 5, 12, 0, 59, true);
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

	if (isLoadingShow) {
		float width = Graphical::GetWidth();
		float height = Graphical::GetHeight();
		float scale = 200.0f;
		loadingAnim.Draw(Vector2(width - scale * 0.5f, scale * 0.5f), 0.0f, Vector2(scale, scale), Color(1.0f, 1.0f, 1.0f, rate.a));
	}
}

bool MoveScene::Move(Color dstRate, bool isLoadingShow) {
	MoveScene::dstRate = dstRate;
	MoveScene::isLoadingShow = isLoadingShow;

	Color direction = MoveScene::dstRate - rate;
	float distance = direction.r * direction.r + direction.g * direction.g + direction.b * direction.b + direction.a * direction.a;
	if (distance <= 0.05f) return true;
	else return false;
}
