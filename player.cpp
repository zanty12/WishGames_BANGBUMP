//--------------------------------------------------------------------------------
// 
// �v���C���[[player.cpp]
// 
// �쐬�� ���{��
// 
// �쐬��		2023/11/17
// �ŏI�X�V��	2023/11/20
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
