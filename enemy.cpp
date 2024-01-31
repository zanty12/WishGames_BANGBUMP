#include "enemy.h"
#include "camera.h"

const float Enemy::BLLINKING_MAX_TIME_ = 1 + (1.0f / 4);

void Enemy::DropSkillOrb(Vector2 pos, SKILLORB_SIZE_TYPE sizeType)
{
	enemy_mngr_->GetMapMngr()->GetGame()->GetSkillOrbMngr()->Pop(pos, sizeType,true);
}

void Enemy::blinking(void)
{
	//----------------------------------------
	// “_–Å
	//----------------------------------------
	blinking_time_ += Time::GetDeltaTime();

	if (blinking_time_ > BLLINKING_MAX_TIME_)
	{
		blinking_time_ = 0.0f;
		flashing_ = false;
		SetColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
		return;
	}

	flash_time_ += Time::GetDeltaTime();

	if (flash_time_ > 0.1f)
	{
		flash_time_ = 0.0f;
		if (GetColor().a == 1.0f)
			SetColor(Color(1.0f, 1.0f, 1.0f, 0.0f));
		else
			SetColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
	}


}


