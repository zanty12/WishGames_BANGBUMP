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

	if (move_attribute_ != nullptr)
	{
		SetVel(move_attribute_->Move());
	}
	else
	{//何も操作しなければ落ちる
		if (GetVel().y <= GRAVITY_SCALE_)
			SetVel(Vector2(GetVel().x, GetVel().y - 0.05f));
	}

	if (attack_attribute_ != nullptr)
	{
		attack_attribute_->Action();
	}

	AddVel(GetVel());
	UpdateDir();
}
