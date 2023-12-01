//--------------------------------------------------------------------------------
// 
// ワープ[fark.h]
// 
// 作成者 趙潤博
// 
// 最終更新日	2023/11/22
// 
//--------------------------------------------------------------------------------
#pragma once
#include "attribute.h"

class Dark : public Attribute
{
	Vector2 warpPosition;
	Vector2 attackDirection;						// 攻撃する向き
	const float maxSpeedFalling = 0.5f;				// 落下中のスピード
	const float warpDistance = 800.0f;				// ワープ距離
	const float attackWidthLength = 5.0f;			// レーザーの幅の長さ
	const float responseMinStickDistance = 0.2f;	// スティックの傾けたときに判定する最小値

	PHYSICS::Vertex4 attackCollider;
	bool isDraw = false;

public:

	Dark(Player *player) : Attribute(player) {}

	Vector2 Move() override;
	void Action() override;
	void Draw(Vector2 offset) override;

};