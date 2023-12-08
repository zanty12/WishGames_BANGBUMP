#include "enemy1.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"

bool CheckLength(Vector2 a, Vector2 b, float len);

enum Enemy_move
{
    ES_MOVE_RIGHT,		// 右に移動する状態
    ES_MOVE_LEFT,		// 左に移動する状態
};
   


void Enemy1::Update()
{

    Player* player = GetEnemyMngr()->GetMapMngr()->GetGame()->GetPlayer(); //正直これのメモリ操作多すぎ

    GetAnimator()->SetIsAnim(true);


    

    //重力
    //SetVel(Vector2(GetVel().x, GetVel().y - y_spd_ * dt));

    
    /*float once = 3.0f;
    float once_time = Time::GetDeltaTime();*/



    if (!CheckLength(GetPos(), startPosition, (SIZE_ * 3.0f)))
    {
        startPosition = GetPos();
        SetVel(Vector2(GetVel().x * -1, GetVel().y));
    }

   

    

   
    /*if (startPosition.x < (SIZE_ * 3.0f))
    {
        SetVel(Vector2(x_spd_ * dt, GetVel().y));
    }*/
    

    this->AddVel(GetVel());

    //壁判定
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
    }
}

void Enemy1::CellActions()
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
}


bool CheckLength(Vector2 a, Vector2 b, float len)
{

    if (Vector2::Distance(a, b) < len)
    {
        return true;
    }

    return false;
}
