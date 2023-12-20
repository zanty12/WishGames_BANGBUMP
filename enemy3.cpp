#include "enemy3.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"

#define RANGE (SIZE_ * 20.0f)                            //範囲


bool CheckEnemy3Length(Vector2 a, Vector2 b, float len);

void Enemy3::Update()
{
    //Player* player = GetEnemyMngr()->GetMapMngr()->GetGame()->GetPlayer(); //正直これのメモリ操作多すぎ


    std::list<Collider*> collisions = GetCollider()->GetCollision();
    for (auto collision : collisions)
    {
        OBJECT_TYPE type = collision->GetParent()->GetType();
        //実際の処理
        if (type == OBJ_PLAYER)
        {
            Player* player = dynamic_cast<Player*> (collision->GetParent());
            player->HpDown(15);//★仮★
        }
    }

    GetAnimator()->SetIsAnim(true);

    float dt = Time::GetDeltaTime(); //初期化時のエラーを回避する

    //重力
    //SetVel(Vector2(GetVel().x, GetVel().y - y_spd_ * dt));

    //プレイヤー追従
    std::list<Player*> players = Game::GetPlayer();

    for (auto player : players)
    {
        RangeEnemy(startPosition.x, startPosition.y, GetPos().x, GetPos().y);
        RangePlayer(startPosition.x, startPosition.y, player->GetPos().x, player->GetPos().y);

        if (cheakRange_Player_ == true)
        {
            if (CheckEnemy3Length(GetPos(), startPosition, RANGE))//仮
            {
                if (cheakRange_Enemy_ == true)
                {
                    Vector2 v = startPosition - GetPos();
                    SetVel(v.Normalize() * spd_ * dt);
                }
            }
            else
            {
                if (cheakRange_Enemy_ == false)
                {

                    Vector2 v = player->GetPos() - GetPos();
                    SetVel(v.Normalize() * spd_ * dt);
                }
            }
        }
        else
        {
            Vector2 v = startPosition - GetPos();
            SetVel(v.Normalize() * spd_ * dt);
        }
    }
      
    //if (CheckEnemy3Length(GetPos(), startPosition, RANGE))//仮
    //{
    //    if (cheakRange == true)
    //    {
    //        Vector2 v = startPosition - GetPos();
    //        SetVel(v.Normalize() * spd_ * dt);
    //    }
    //}
    //else
    //{
    //    if (cheakRange == false)
    //    {

    //        Vector2 v = player->GetPos() - GetPos();
    //        SetVel(v.Normalize() * spd_ * dt);
    //    }
    //}

    //他の敵との当たり判定
   /* for (auto& enemy : GetEnemyMngr()->GetEnemies())
    {
        if (enemy == this || enemy == nullptr)
            continue;
        if (Collision(enemy))
        {
            enemy->SetVel(Vector2(enemy->GetVel().x, enemy->GetVel().y ));
        }
    }*/

    this->AddVel(GetVel());
}

/*
void Enemy3::CellActions()
{
    Map* map = GetEnemyMngr()->GetMapMngr()->GetMap();
    Cell* cells[4] = { nullptr, nullptr, nullptr, nullptr };
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
*/

bool CheckEnemy3Length(Vector2 a, Vector2 b, float len)
{

    if (Vector2::Distance(a, b) > len)
    {
        return true;
    }

    return false;
}
bool CheckEnemy3Length(Vector2 a, Vector2 b, float len)
{

    if (Vector2::Distance(a, b) > len)
    {
        return true;
    }

    return false;
}

void Enemy3::RangeEnemy(float a, float b, float c, float d)
{
    // 円の情報
    float x = a;
    float y = b;
    float radius_0 = RANGE;
    float radius_1 = 0.5f;

    float h = c - x;
    float i = d - y;
    float g = sqrt(h * h + i * i);

    if (g > radius_0)
    {
        cheakRange_Enemy_ = true;
    }
    else if (g < radius_1)
    {
        cheakRange_Enemy_ = false;
    }
}
void Enemy3::RangePlayer(float a, float b, float c, float d)
{
    // 円の情報
    float x = a;
    float y = b;
    float radius_0 = RANGE;

    float h = c - x;
    float i = d - y;
    float g = sqrt(h * h + i * i);

    if (g < radius_0)
    {
        cheakRange_Player_ = true;
    }
    else if (g > radius_0)
    {
        cheakRange_Player_ = false;
    }
}

