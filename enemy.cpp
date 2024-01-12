#include "enemy.h"
#include "camera.h"

void Enemy::DropSkillOrb(Vector2 pos, SKILLORB_SIZE_TYPE sizeType)
{
	enemy_mngr_->GetMapMngr()->GetGame()->GetSkillOrbMngr()->Pop(pos, sizeType,true);
}

void Enemy::blinking()
{
	//----------------------------------------
	// “_–Å
	//----------------------------------------
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


