#include "enemy3.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"

#define RANGE (SIZE_ * 20.0f)                            //”ÍˆÍ


bool CheckEnemy3Length(Vector2 a, Vector2 b, float len);
bool CheckPlayerLength(Vector2 a, Vector2 b, float len);

void Enemy3::Update()
{
    //HP‚ª0‚É‚È‚Á‚½‚çÁ‚·
    if (GetHp() <= 0)
    {
        GameObject::Discard();
        Die();
    }

    std::list<Collider*> collisions = GetCollider()->GetCollision();
    for (auto collision : collisions)
    {
        OBJECT_TYPE type = collision->GetParent()->GetType();
        ////ÀÛ‚Ìˆ—
        //if (type == OBJ_PLAYER)
        //{
        //    Player* player = dynamic_cast<Player*> (collision->GetParent());
        //    player->HpDown(15);//š‰¼š
        //}
    }



    GetAnimator()->SetIsAnim(true);

    float dt = Time::GetDeltaTime(); //‰Šú‰»‚ÌƒGƒ‰[‚ğ‰ñ”ğ‚·‚é

    //d—Í
    //SetVel(Vector2(GetVel().x, GetVel().y - y_spd_ * dt));

    //ƒvƒŒƒCƒ„[’Ç]
    std::list<Player*> players = GetEnemyMngr()->GetMapMngr()->GetGame()->GetPlayers();

    float Spos_now = 0.0f;
    float Spos_old = 0.0f;
    Player* close_player = nullptr;

    for (auto player : players)
    {
        RangeEnemy(startPosition.x, startPosition.y, GetPos().x, GetPos().y);
        RangePlayer(startPosition.x, startPosition.y, player->GetPos().x, player->GetPos().y);

        if (CheckPlayerLength(startPosition, player->GetPos(), RANGE))
        {
            Spos_now = Vector2::Distance(GetPos(), player->GetPos());
            if (Spos_now < Spos_old)
            {
                close_player = player;
                Spos_old = Spos_now;

            }
        }


        if (cheakRange_Player_ == true)
        {
            if (CheckEnemy3Length(GetPos(), startPosition, RANGE))//‰¼
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
      
    //if (CheckEnemy3Length(GetPos(), startPosition, RANGE))//‰¼
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

    //‘¼‚Ì“G‚Æ‚Ì“–‚½‚è”»’è
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

SkillOrb* Enemy3::DropSkillOrb()
{
    if (GetDiscard() == false)
        return nullptr;

    switch (rand()%4)
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

    return new SkillOrb(GetPos(), drop, SKILLORB_SIZE_DESC::Big());
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
        //if (Collision(cells[i]))
        //    MapCellInteract(cells[i]);
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

bool CheckPlayerLength(Vector2 a, Vector2 b, float len)
{

    if (Vector2::Distance(a, b) < len)
    {
        return true;
    }

    return false;
}

void Enemy3::RangeEnemy(float a, float b, float c, float d)
{
    // ‰~‚Ìî•ñ
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
    // ‰~‚Ìî•ñ
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

