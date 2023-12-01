#pragma once
#include"attribute.h"

class Fire :public Attribute
{
	const float speed = 10.0f;						// スピード
	float friction = 0.88f;							// 摩擦定数
	const float attackInjectionLength = 20.0f;		// 炎の射出長さ
	const float attackWidthLength = 5.0f;			// 炎の幅の長さ
	const float responseMinStickDistance = 0.2f;	// スティックの傾けたときに判定する最小値

	Vector2 attackDirection;
	bool isDraw = false;

public:
	Fire(Player *player) : Attribute(player) { }
	Vector2 Move() override;
	void Action() override;
	void Draw(Vector2 offset)override;
};