//--------------------------------------------------------------------------------
// 
// �T���_�[[thunder.cpp]
// 
// �쐬�� �⏁��
// 
// �ŏI�X�V��	2023/11/22
// 
//--------------------------------------------------------------------------------
#include "thunder.h"
#include"xinput.h"
#include"lib/collider2d.h"

Vector2 Thunder::Move()
{
	if (Input::GetPreviousStickLeft(0) != Vector2(0, 0) && Input::GetStickLeft(0) == Vector2(0, 0))
	{
		Vector2 vel = player_.GetVel();
		Vector2 stick = Input::GetStickLeft(0);
		//Vector2 
	}

	
}

void Thunder::Action()
{
}
