//--------------------------------------------------------------------------------
// 
// ���[�v[fark.h]
// 
// �쐬�� �⏁��
// 
// �ŏI�X�V��	2023/11/22
// 
//--------------------------------------------------------------------------------
#pragma once
#include "attribute.h"

class Dark : public Attribute
{

	bool is_trigger_;

	bool is_press_;


public:

	Dark(Player& player) : Attribute(player){}

	Vector2 Move() override;

	void Action() override;
	
};