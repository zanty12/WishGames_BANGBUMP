//--------------------------------------------------------------------------------
// 
// �T���_�[[thunder.h]
// 
// �쐬�� �⏁��
// 
// �ŏI�X�V��	2023/11/22
// 
//--------------------------------------------------------------------------------
#pragma once
#include "attribute.h"

class Thunder : public Attribute
{



public:
   
    Thunder(Player& player) : Attribute(player) {}
    Vector2 Move() override;
    void Action() override;

};

