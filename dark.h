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
	float maxSpeedFalling = 0.5f;		// 落下中のスピード
	float warpDistance = 800.0f;			// ワープ距離

public:

	Dark(Player *player) : Attribute(player) {}

	Vector2 Move() override;

	void Action() override;

};