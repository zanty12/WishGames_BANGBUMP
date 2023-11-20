//--------------------------------------------------------------------------------
// 
// プレイヤー[player.cpp]
// 
// 作成者 根本賢
// 
// 作成日		2023/11/17
// 最終更新日	2023/11/20
// 
//--------------------------------------------------------------------------------

#include "player.h"

bool Player::UseSkillPoint(void)
{
	if (skillpt_ == SKILL_GAUGE_MAX_)
	{
		skillpt_ = 0;
		return true;
	}

	return false;
}

void Player::Update(void)
{

	if (attribute_ != nullptr)
	{
		SetVel(attribute_->Move());
	}
	else
	{
		if (GetVel().y <= GRAVITY_SCALE_)
			SetVel(Vector2(GetVel().x, GetVel().y + 0.05f));
	}

	AddVel(GetVel());
	UpdateDir();
}
