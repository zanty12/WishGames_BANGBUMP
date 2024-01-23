#include "enemy.h"
#include "camera.h"

const float Enemy::BLLINKING_MAX_TIME_ = 1 + (1.0f / 4);

void Enemy::DropSkillOrb(Vector2 pos, SKILLORB_SIZE_TYPE sizeType)
{
	enemy_mngr_->GetMapMngr()->GetGame()->GetSkillOrbMngr()->Pop(pos, sizeType,true);
}

void Enemy::blinking(GameObject* obj)
{
	//----------------------------------------
	// 点滅
	//----------------------------------------
	blinking_time_ += Time::GetDeltaTime();

	if (blinking_time_ > BLLINKING_MAX_TIME_)
	{
		SetColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
		return;
	}
	//★アタッククラスができ次第★
	GameObject* attack = obj;
	//何かしらのアタッククラス* attack = dynamic_cast<何かしらのアタッククラス*>(obj)
	//if (何かしらのアタッククラス == nullptr)
	//{
	//	return;
	//}
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

void Enemy::blinking(PlayerAttack* obj)
{
	//----------------------------------------
	// 点滅
	//----------------------------------------
	blinking_time_ += Time::GetDeltaTime();

	if (blinking_time_ > BLLINKING_MAX_TIME_)
	{
		SetColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
		return;
	}
	//★アタッククラスができ次第★
	PlayerAttack* attack = obj;
	//何かしらのアタッククラス* attack = dynamic_cast<何かしらのアタッククラス*>(obj)
	//if (何かしらのアタッククラス == nullptr)
	//{
	//	return;
	//}
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


