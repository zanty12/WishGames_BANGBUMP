//--------------------------------------------------------------------------------
// 
// サンダー[thunder.cpp]
// 
// 作成者 趙潤博
// 
// 最終更新日	2023/11/22
// 
//--------------------------------------------------------------------------------
#include "thunder.h"
#include"xinput.h"
#include"lib/collider2d.h"

Vector2 Thunder::Move()
{
	Vector2 stick = Input::GetStickLeft(0);
	Vector2 previousStick = Input::GetPreviousStickLeft(0);
	stick.y *= -1;
	previousStick.y *= -1;
	float distance = stick.Distance();


	if (responseMinStickDistance <= distance) {
		charge += distance;
		if (maxChage < charge) charge = maxChage;
	}

	if (previousStick != Vector2::Zero && stick == Vector2::Zero && responseMinOneFrameDistance < (stick - previousStick).Distance()) {

		Vector2 direction = -previousStick * charge;
		return direction;
	}

	return player_->GetVel();
}

void Thunder::Action()
{

}
