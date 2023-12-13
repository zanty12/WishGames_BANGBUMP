#include "enemy1.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"

bool CheckLength(Vector2 a, Vector2 b, float len);

void Enemy1::Update()
{
    Player* player = GetEnemyMngr()->GetMapMngr()->GetGame()->GetPlayer(); //正直これのメモリ操作多すぎ
    

    GetAnimator()->SetIsAnim(true);

    if (!CheckLength(GetPos(), startPosition, (SIZE_ * 3.0f))/* || CellActions()*/)
    {
        startPosition = GetPos();
        SetVel(Vector2(GetVel().x * -1, GetVel().y));
    }

    this->AddVel(GetVel());

    //壁判定
    CellAction();

    //プレイヤーとの当たり判定
    if (Collision(player))
    {
        player->HpDown(3);//★仮★
        //Die();
    }
    ////他の敵との当たり判定
    //for (auto& enemy : GetEnemyMngr()->GetEnemies())
    //{
    //    if (enemy == this || enemy == nullptr)
    //        continue;
    //    if (Collision(enemy))
    //    {
    //        enemy->SetVel(Vector2(enemy->GetVel().x, 0.0f));
    //    }
    //}


}

void Enemy1::CellAction()
{
    Map* map = GetEnemyMngr()->GetMapMngr()->GetMap();
    Cell* cells[4] = { nullptr, nullptr, nullptr, nullptr };
    int idx = std::floor((GetPos().x / SIZE_));
    int idy = std::floor((GetPos().y / SIZE_));
    cells[0] = map->GetCell(idx, idy + 1);
    cells[1] = map->GetCell(idx, idy - 1);
    cells[2] = map->GetCell(idx - 1, idy);
    cells[3] = map->GetCell(idx + 1, idy);
    for (int i = 0; i < 4; i++)
    {
        if (cells[i] == nullptr)
            continue;
        if (Collision(cells[i]))
        {
            switch (i)
            {
            case 0:
                if (GetVel().y > 0)
                {
                    SetVel(Vector2(GetVel().x, GetVel().y * -1));
                }
                break;
            case 1:
                if (GetVel().y < 0)
                {
                    SetVel(Vector2(GetVel().x, GetVel().y * -1));
                }
                break;
            case 2:
                if (GetVel().x < 0)
                {
                    SetVel(Vector2(GetVel().x * -1, GetVel().y));
                }
                break;
            case 3:
                if (GetVel().x > 0)
                {
                    SetVel(Vector2(GetVel().x * -1, GetVel().y));
                }
                break;
            default:
                break;
            }
            break;
        }
    }
}

void Enemy1::CellActionSpike()
{
    Map* map = GetEnemyMngr()->GetMapMngr()->GetMap();
    Cell* cells[4] = { nullptr, nullptr, nullptr, nullptr };
    int idx = std::floor((GetPos().x / SIZE_));
    int idy = std::floor((GetPos().y / SIZE_));
    cells[0] = map->GetCell(idx, idy + 1);
    cells[1] = map->GetCell(idx, idy - 1);
    cells[2] = map->GetCell(idx - 1, idy);
    cells[3] = map->GetCell(idx + 1, idy);

    for (int i = 0; i < 4; i++)
    {
        if (cells[i] == nullptr)
            continue;

        //トゲの時の処理
        if (Collision(cells[i]))
        {
            MAP_READ cell_type = cells[i]->GetCellType();
            if (cell_type == MAP_READ_SPIKE_UP || cell_type == MAP_READ_SPIKE_DOWN)
            {
                switch (i)
                {
                case 0:
                    if (GetVel().y > 0)
                    {
                        SetVel(Vector2(GetVel().x, GetVel().y * -1));
                    }
                    break;
                case 1:
                    if (GetVel().y < 0)
                    {
                        SetVel(Vector2(GetVel().x, GetVel().y * -1));
                    }
                    break;
                case 2:
                    if (GetVel().x < 0 || cell_type == MAP_READ_SPIKE_LEFT)
                    {
                        SetVel(Vector2(GetVel().x * -1, GetVel().y));
                        enemy->

                    }
                    break;
                case 3:
                    if (GetVel().x > 0)
                    {
                        SetVel(Vector2(GetVel().x * -1, GetVel().y));
                    }
                    break;
                }
                break;
            }

        }

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