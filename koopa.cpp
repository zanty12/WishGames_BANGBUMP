#include "koopa.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"

void Koopa::Update()
{
    Player* player = GetEnemyMngr()->GetMapMngr()->GetGame()->GetPlayer(); //正直これのメモリ操作多すぎ

    GetAnimator()->SetIsAnim(true);

    float dt = Time::GetDeltaTime() < 1 ? Time::GetDeltaTime() : 0.0f; //初期化時のエラーを回避する

    //重力
    SetVel(Vector2(GetVel().x, GetVel().y - y_spd_ * dt));

    //プレイヤー追従
    if (player->GetPos().x < GetPos().x)
    {
        SetVel(Vector2(-x_spd_ * dt, GetVel().y));
    }
    else if (player->GetPos().x > GetPos().x)
    {
        SetVel(Vector2(x_spd_ * dt, GetVel().y));
    }
    else
    {
        SetVel(Vector2(0.0f, GetVel().y));
    }

    this->AddVel(GetVel());

    /*//壁判定
    CellActions();
    //プレイヤーとの当たり判定
    if (Collision(player))
    {
        player->HpDown(500);//★仮★
        //Die();
    }
    //他の敵との当たり判定
    for (auto& enemy : GetEnemyMngr()->GetEnemies())
    {
        if (enemy == this || enemy == nullptr)
            continue;
        if (Collision(enemy))
        {
            //enemy->SetVel(Vector2(enemy->GetVel().x, 0.5f));
        }
    }*/
}

/*void Koopa::CellActions()
{
    Map* map = GetEnemyMngr()->GetMapMngr()->GetMap();
    Cell* cells[4] = {nullptr, nullptr, nullptr, nullptr};
    int idx = std::floor(GetPos().x / SIZE_);
    int idy = std::floor(GetPos().y / SIZE_);
    cells[0] = map->GetCell(idx, idy + 1);
    cells[1] = map->GetCell(idx, idy - 1);
    cells[2] = map->GetCell(idx - 1, idy);
    cells[3] = map->GetCell(idx + 1, idy);
    for (int i = 0; i < 4; i++)
    {
        if (cells[i] == nullptr)
            continue;
        if (Collision(cells[i]))
            MapCellInteract(cells[i]);
    }
}*/
