#include "multi_character_select_frame.h"

void CharacterSelectFrameClientSide::Draw(int frameTexNo, int framePTexNo, int arrowTexNo, int playerTexNo, int bootTexNo, int handTexNo, bool isShow, float width, float height, float gap, int moveAttribute, int attackAttribute) {
	const float SCREEN_WIDTH = Graphical::GetWidth();					// ��ʂ̕�
	const float SCREEN_HEIGHT = Graphical::GetHeight();					// ��ʂ̍���
	const float center = (float)maxNum - (float)maxNum * 0.5f - 0.5f;	// ����

	// �����̒l���X�V
	moveSmooth = moveAttribute;
	attackSmooth = attackAttribute;
	moveSmooth.update();
	attackSmooth.update();
	stateSmooth.update();

	// ���W�����߂�
	float x = (SCREEN_WIDTH * 0.5f) - (center - id) * (width + gap);
	float y = (SCREEN_HEIGHT * 0.5f);

	// ��������A�j���[�V���������߂�i�G���g���[�ҁj
	float entryRate = stateSmooth;
	if (1.0f < entryRate) entryRate = 1.0f;
	float scl = MATH::Leap(2.0f, 1.0f, entryRate * entryRate);
	float alpha = MATH::Leap(0.0f, 1.0f, entryRate * entryRate);

	// ��������A�j���[�V���������߂�i�L��������ҁj
	float okRate = stateSmooth - 1.0f;
	if (1.0f < okRate) okRate = 1.0f;
	else if (0.0f > okRate) okRate = 0.0f;
	float col = MATH::Leap(1.0f, 0.25f, okRate * okRate);




	float scaleMoveAttributeRate = MATH::Abs(moveAttribute - moveSmooth) + 1.0f;
	float scaleAttackAttributeRate = MATH::Abs(attackAttribute - attackSmooth) + 1.0f;

	// �`��i�t���[���j
	DrawSprite(frameTexNo, Vector2(x, y), 0.0f, Vector2::One * (height + 300), Color::White);
	// �`��i�L�����j
	if (isShow) {
		Vector2 scale = Vector2::One * height;
		// �t���[��
		DrawSprite(framePTexNo, Vector2(x, y), 0.0f, Vector2::One * (height + 300) * scl, Color(1,1,1, alpha));
		// �L����
		DrawSprite(playerTexNo, Vector2(x, y + 40), 0.0f, Vector2::One * height * scl, Color(1, 1, 1, alpha) * col);
		// ��
		DrawSprite(bootTexNo, Vector2(x, y + 40), 0.0f, Vector2::One * height * scl * scaleMoveAttributeRate, Color(1, 1, 1, alpha) * col);
		// �r
		DrawSprite(handTexNo, Vector2(x, y + 40), 0.0f, Vector2::One * height * scl * scaleAttackAttributeRate, Color(1, 1, 1, alpha) * col);
		// ���
		DrawSprite(arrowTexNo, Vector2(x + 160, y + 40), 0.0f, Vector2::One * 50 * scl * scaleAttackAttributeRate, Color(1, 1, 1, alpha) * col);
		DrawSprite(arrowTexNo, Vector2(x - 160, y + 40), 3.1415f, Vector2::One * 50 * scl * scaleAttackAttributeRate, Color(1, 1, 1, alpha) * col, Vector2(0.0f, 1.0f), Vector2(1.0f, -1.0f));
		DrawSprite(arrowTexNo, Vector2(x + 160, y - 180), 0.0f, Vector2::One * 50 * scl * scaleAttackAttributeRate, Color(1, 1, 1, alpha) * col);
		DrawSprite(arrowTexNo, Vector2(x - 160, y - 180), 3.1415f, Vector2::One * 50 * scl * scaleAttackAttributeRate, Color(1, 1, 1, alpha) * col, Vector2(0.0f, 1.0f), Vector2(1.0f, -1.0f));

	}
}
