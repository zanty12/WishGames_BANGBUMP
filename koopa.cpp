#include "koopa.h"

void Koopa::Update()
{
    Player* player = GetEnemyMngr()->GetMapMngr()->GetGame()->GetPlayer(); //正直これのメモリ操作多すぎ
    //Get to the player only horizontally
    if (player->GetPos().x < GetPos().x)
    {
        SetVel(Vector2(-0.5f, GetVel().y));
    }
    else if (player->GetPos().x > GetPos().x)
    {
        SetVel(Vector2(0.5f, GetVel().y));
    }
    else
    {
        SetVel(Vector2(0.0f, GetVel().y));
    }
    this->AddVel(GetVel());
}

