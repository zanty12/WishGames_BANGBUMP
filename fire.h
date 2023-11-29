#pragma once
#include"attribute.h"

class Fire :public Attribute
{
	float speed = 10.0f;		// �X�s�[�h
	float friction = 0.88f;		// ���C�萔
	const float responseMinStickDistance = 0.2f;	// �X�e�B�b�N�̌X�����Ƃ��ɔ��肷��ŏ��l

public:
	Fire(Player *player) : Attribute(player) { }
	Vector2 Move()override;
	void Action()override;
};