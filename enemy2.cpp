#include "enemy2.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"

bool CheckLength(Vector2 a, Vector2 b, float len);

void Enemy2::Update()
{
    Player* player = GetEnemyMngr()->GetMapMngr()->GetGame()->GetPlayer(); //³’¼‚±‚ê‚Ìƒƒ‚ƒŠ‘€ì‘½‚·‚¬


    GetAnimator()->SetIsAnim(true);

    //•Ç”»’è
    CellActions();

    //ƒvƒŒƒCƒ„[‚Æ‚Ì“–‚½‚è”»’è
    if (Collision(player))
    {
        player->HpDown(3);//š‰¼š
        //Die();
    }

    //‘¼‚Ì“G‚Æ‚Ì“–‚½‚è”»’è
 /*   for (auto& enemy : GetEnemyMngr()->GetEnemies())
    {
        if (enemy == this || enemy == nullptr)
            continue;
        if (Collision(enemy))
        {
            enemy->SetVel(Vector2(enemy->GetVel().x * -1, 0.0f));
        }
    }*/




    this->AddVel(GetVel());
}

void Enemy2::CellActions()
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