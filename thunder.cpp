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
	Vector2 vel = player_.GetVel();
	Vector2 stick = Input::GetStickLeft(0);
	Vector2 pstick = Input::GetPreviousStickLeft(0);

	if (Input::GetPreviousStickLeft(0) != Vector2(0, 0) && Input::GetStickLeft(0) == Vector2(0, 0))
	{
		player_.SetVel(-stick * 10);
	}

	return Vector2::Zero;
}

void Thunder::Action()
{

}
