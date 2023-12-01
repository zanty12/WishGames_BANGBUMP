#pragma once
#include"attribute.h"

class Fire :public Attribute
{
	const float speed = 10.0f;						// �X�s�[�h
	float friction = 0.88f;							// ���C�萔
	const float attackInjectionLength = 20.0f;		// ���̎ˏo����
	const float attackWidthLength = 5.0f;			// ���̕��̒���
	const float responseMinStickDistance = 0.2f;	// �X�e�B�b�N�̌X�����Ƃ��ɔ��肷��ŏ��l

	Vector2 attackDirection;
	bool isDraw = false;

public:
	Fire(Player *player) : Attribute(player) { }
	Vector2 Move() override;
	void Action() override;
	void Draw(Vector2 offset)override;
};