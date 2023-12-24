#include "enemy2.h"
#include "bullet.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"

#define RANGE (SIZE_ * 10.0f)                            //”ÍˆÍ


bool CheckEnemy2Length(Vector2 a, Vector2 b, float len);

void Enemy2::Update()
{
    std::list<Collider*> collisions = GetCollider()->GetCollision();
    for (auto collision : collisions)
    {
        OBJECT_TYPE type = collision->GetParent()->GetType();
    }

    if (hp_ <= 0)
    {
        GameObject::Discard();
    }

    GetAnimator()->SetIsAnim(true);

    //std::list<Player*> players = GetEnemyMngr()->GetMapMngr()->GetGame()->GetPlayers();

    //for (auto player : players)
    //{
    //    if (CheckEnemy2Length(startPosition, player->GetPos(), RANGE))
    //    {
    //        

    //    }
    //}




    /*//•Ç”»’è
    CellActions();

    //ƒvƒŒƒCƒ„[‚Æ‚Ì“–‚½‚è”»’è
    if (Collision(player))
    {
        player->HpDown(3);//š‰¼š
        //Die();
    }*/

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





}

//void Enemy2::Spawn(int x, int y, int type)
//{
//    //TODO: ‚±‚±‚Å’e‚ð¶¬‚·‚é
//    if (type == OBJ_ENEMY_BULLET)
//    {
//        Bullet* bullet = new Bullet(x, y, this);
//    }
//}

/*
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
}*/

bool CheckEnemy2Length(Vector2 a, Vector2 b, float len)
{

    if (Vector2::Distance(a, b) < len)
    {
        return true;
    }

    return false;
}

