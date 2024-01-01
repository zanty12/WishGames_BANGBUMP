#include "enemy1.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"

bool CheckLength(Vector2 a, Vector2 b, float len);

void Enemy1::Update()
{
    //HPが0になったら消す
    if (GetHp() <= 0)
    {
        GameObject::Discard();
        Die();
    }

    std::list<Collider*> collisions = GetCollider()->GetCollision();
   
    for (auto collision : collisions)
    {
        OBJECT_TYPE type = collision->GetParent()->GetType();

        ////実際の処理
        //if (type == OBJ_PLAYER)
        //{
        //    Player* player =dynamic_cast<Player*> (collision->GetParent());
        //    player->HpDown(15);//★仮★
        //}
    }

    GetAnimator()->SetIsAnim(true);

    if (!CheckLength(GetPos(), startPosition, (SIZE_ * 3.0f)))
    {
        startPosition = GetPos();
        SetVel(Vector2(GetVel().x * -1, GetVel().y));
    }

    //他の敵との当たり判定
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

SkillOrb* Enemy1::DropSkillOrb()
{
    if (GetDiscard() == false)
        return nullptr;

    switch (rand() % 4)
    {
    case 0:
        drop = SKILLORB_ATTRIBUTE_DESC::Fire();
        break;
    case 1:
        drop = SKILLORB_ATTRIBUTE_DESC::Dark();
        break;
    case 2:
        drop = SKILLORB_ATTRIBUTE_DESC::Wind();
        break;
    case 3:
        drop = SKILLORB_ATTRIBUTE_DESC::Thunder();
        break;
    default:
        break;
    }

    return new SkillOrb(GetPos(), drop, SKILLORB_SIZE_DESC::Small());
}

/*void Enemy1::CellAction()
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
        //if (Collision(cells[i]))
        //{
        //    switch (i)
        //    {
        //    case 0:
        //        if (GetVel().y > 0)
        //        {
        //            SetVel(Vector2(GetVel().x, GetVel().y * -1));
        //        }
        //        break;
        //    case 1:
        //        if (GetVel().y < 0)
        //        {
        //            SetVel(Vector2(GetVel().x, GetVel().y * -1));
        //        }
        //        break;
        //    case 2:
        //        if (GetVel().x < 0)
        //        {
        //            SetVel(Vector2(GetVel().x * -1, GetVel().y));
        //        }
        //        break;
        //    case 3:
        //        if (GetVel().x > 0)
        //        {
        //            SetVel(Vector2(GetVel().x * -1, GetVel().y));
        //        }
        //        break;
        //    default:
        //        break;
        //    }
        //    break;
        //}
    }
}

void Enemy1::CellActionSpike()
{
    //Enemy1* enemy;

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
                    if (GetVel().x < 0/* || cell_type == MAP_READ_SPIKE_LEFT#1#/* && cell_type != MAP_READ_SPIKE_RIGHT#1#)
                    {
                        SetVel(Vector2(GetVel().x * -1, GetVel().y));
                       /* if (Collision(enemy))
                        {
                            enemy->HpDown(3);
                        }#1#
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

        //}

    }
}*/




bool CheckLength(Vector2 a, Vector2 b, float len)
{

    if (Vector2::Distance(a, b) < len)
    {
        return true;
    }

    return false;
}