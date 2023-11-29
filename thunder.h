//--------------------------------------------------------------------------------
// 
// サンダー[thunder.h]
// 
// 作成者 趙潤博
// 
// 最終更新日	2023/11/22
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

