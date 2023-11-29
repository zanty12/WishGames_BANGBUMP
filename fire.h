#pragma once
#include"attribute.h"

class Fire :public Attribute
{
	float speed = 10.0f;		// スピード
	float friction = 0.88f;		// 摩擦定数
	const float responseMinStickDistance = 0.2f;	// スティックの傾けたときに判定する最小値

public:
	Fire(Player *player) : Attribute(player) { }
	Vector2 Move()override;
	void Action()override;
};