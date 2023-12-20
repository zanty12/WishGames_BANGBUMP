#include "enemy3.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"

bool CheckEnemy3Length(Vector2 a, Vector2 b, float len);
bool CheakRange(Vector2 a, Vector2 b);

void Enemy3::Update()
{
    Player* player = GetEnemyMngr()->GetMapMngr()->GetGame()->GetPlayer(); //正直これのメモリ操作多すぎ

    GetAnimator()->SetIsAnim(true);

    float dt = Time::GetDeltaTime() < 1 ? Time::GetDeltaTime() : 0.0f; //初期化時のエラーを回避する

    //重力
    //SetVel(Vector2(GetVel().x, GetVel().y - y_spd_ * dt));

    //プレイヤー追従
    if (CheckEnemy3Length(GetPos(), startPosition, (SIZE_ * 3.0f)))//仮
    {
        if (CheakRange(GetPos(), startPosition))
        {
            if (startPosition.x > GetPos().x && startPosition.y > GetPos().y)
            {
                SetVel(Vector2(x_spd_ * dt, y_spd_ * dt));
            }
            else if (startPosition.x > GetPos().x && startPosition.y < GetPos().y)
            {
                SetVel(Vector2(x_spd_ * dt, -y_spd_ * dt));
            }
            else if (startPosition.x < GetPos().x && startPosition.y > GetPos().y)
            {
                SetVel(Vector2(-x_spd_ * dt, y_spd_ * dt));
            }
            else if (startPosition.x < GetPos().x && startPosition.y < GetPos().y)
            {
                SetVel(Vector2(-x_spd_ * dt, -y_spd_ * dt));
            }
            else if (startPosition.y < GetPos().y)
            {
                SetVel(Vector2(GetVel().x, -y_spd_ * dt));
            }
            else if (startPosition.y > GetPos().y)
            {
                SetVel(Vector2(GetVel().x, y_spd_ * dt));
            }
            else if (startPosition.x < GetPos().x)
            {
                SetVel(Vector2(-x_spd_ * dt, GetVel().y));
            }
            else if (startPosition.x > GetPos().x)
            {
                SetVel(Vector2(x_spd_ * dt, GetVel().y));
            }
        }
    }
    /*else
    {
        if (player->GetPos().x > GetPos().x && player->GetPos().y > GetPos().y)
        {
            SetVel(Vector2(x_spd_ * dt, y_spd_ * dt));
        }
        else if (player->GetPos().x > GetPos().x && player->GetPos().y < GetPos().y)
        {
            SetVel(Vector2(x_spd_ * dt, -y_spd_ * dt));
        }
        else if (player->GetPos().x < GetPos().x && player->GetPos().y > GetPos().y)
        {
            SetVel(Vector2(-x_spd_ * dt, y_spd_ * dt));
        }
        else if (player->GetPos().x < GetPos().x && player->GetPos().y < GetPos().y)
        {
            SetVel(Vector2(-x_spd_ * dt, -y_spd_ * dt));
        }
        else if (player->GetPos().y < GetPos().y)
        {
            SetVel(Vector2(GetVel().x, -y_spd_ * dt));
        }
        else if (player->GetPos().y > GetPos().y)
        {
            SetVel(Vector2(GetVel().x, y_spd_ * dt));
        }
        else if (player->GetPos().x < GetPos().x)
        {
            SetVel(Vector2(-x_spd_ * dt, GetVel().y));
        }
        else if (player->GetPos().x > GetPos().x)
        {
            SetVel(Vector2(x_spd_ * dt, GetVel().y));
        }
    }*/

    //壁判定
    CellActions();

    //プレイヤーとの当たり判定
    if (Collision(player))
    {
        player->HpDown(15);//★仮★
        //Die();
    }

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

bool CheckEnemy3Length(Vector2 a, Vector2 b, float len)
{

    if (Vector2::Distance(a, b) > len)
    {
        return true;
    }

    return false;
}
bool CheakRange(Vector2 a, Vector2 b)
{
    if (a != b)
    {
        return true;
    }
    else if (a == b)
    {
        return false;
    }
}
