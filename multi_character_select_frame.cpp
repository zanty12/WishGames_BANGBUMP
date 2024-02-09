#include "multi_character_select_frame.h"

void CharacterSelectFrameClientSide::Draw(int frameTexNo, int framePTexNo, int arrowTexNo, int playerTexNo, int bootTexNo, int handTexNo, bool isShow, float width, float height, float gap, int moveAttribute, int attackAttribute) {
	const float SCREEN_WIDTH = Graphical::GetWidth();					// 画面の幅
	const float SCREEN_HEIGHT = Graphical::GetHeight();					// 画面の高さ
	const float center = (float)maxNum - (float)maxNum * 0.5f - 0.5f;	// 中央

	// 属性の値を更新
	moveSmooth = moveAttribute;
	attackSmooth = attackAttribute;
	moveSmooth.update();
	attackSmooth.update();
	stateSmooth.update();

	// 座標をきめる
	float x = (SCREEN_WIDTH * 0.5f) - (center - id) * (width + gap);
	float y = (SCREEN_HEIGHT * 0.5f);

	// 割合からアニメーションを決める（エントリー編）
	float entryRate = stateSmooth;
	if (1.0f < entryRate) entryRate = 1.0f;
	float scl = MATH::Leap(2.0f, 1.0f, entryRate * entryRate);
	float alpha = MATH::Leap(0.0f, 1.0f, entryRate * entryRate);

	// 割合からアニメーションを決める（キャラ決定編）
	float okRate = stateSmooth - 1.0f;
	if (1.0f < okRate) okRate = 1.0f;
	else if (0.0f > okRate) okRate = 0.0f;
	float col = MATH::Leap(1.0f, 0.25f, okRate * okRate);




	float scaleMoveAttributeRate = MATH::Abs(moveAttribute - moveSmooth) + 1.0f;
	float scaleAttackAttributeRate = MATH::Abs(attackAttribute - attackSmooth) + 1.0f;

	// 描画（フレーム）
	DrawSprite(frameTexNo, Vector2(x, y), 0.0f, Vector2::One * (height + 300), Color::White);
	// 描画（キャラ）
	if (isShow) {
		Vector2 scale = Vector2::One * height;
		// フレーム
		DrawSprite(framePTexNo, Vector2(x, y), 0.0f, Vector2::One * (height + 300) * scl, Color(1,1,1, alpha));
		// キャラ
		DrawSprite(playerTexNo, Vector2(x, y + 40), 0.0f, Vector2::One * height * scl, Color(1, 1, 1, alpha) * col);
		// 足
		DrawSprite(bootTexNo, Vector2(x, y + 40), 0.0f, Vector2::One * height * scl * scaleMoveAttributeRate, Color(1, 1, 1, alpha) * col);
		// 腕
		DrawSprite(handTexNo, Vector2(x, y + 40), 0.0f, Vector2::One * height * scl * scaleAttackAttributeRate, Color(1, 1, 1, alpha) * col);
		// 矢印
		DrawSprite(arrowTexNo, Vector2(x + 160, y + 40), 0.0f, Vector2::One * 50 * scl * scaleAttackAttributeRate, Color(1, 1, 1, alpha) * col);
		DrawSprite(arrowTexNo, Vector2(x - 160, y + 40), 3.1415f, Vector2::One * 50 * scl * scaleAttackAttributeRate, Color(1, 1, 1, alpha) * col, Vector2(0.0f, 1.0f), Vector2(1.0f, -1.0f));
		DrawSprite(arrowTexNo, Vector2(x + 160, y - 180), 0.0f, Vector2::One * 50 * scl * scaleAttackAttributeRate, Color(1, 1, 1, alpha) * col);
		DrawSprite(arrowTexNo, Vector2(x - 160, y - 180), 3.1415f, Vector2::One * 50 * scl * scaleAttackAttributeRate, Color(1, 1, 1, alpha) * col, Vector2(0.0f, 1.0f), Vector2(1.0f, -1.0f));

	}
}
