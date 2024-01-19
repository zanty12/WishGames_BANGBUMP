#include "enemy3.h"
#include "Cell.h"
#include "MapMngr.h"
#include "playerattack.h"
#include "lib/collider2d.h"
#include "time.h"

#define RANGE (SIZE_ * 20.0f)                            //範囲


bool CheckEnemy3Length(Vector2 a, Vector2 b, float len);
bool CheckPlayerLength(Vector2 a, Vector2 b, float len);

void Enemy3::Update()
{
    //HPが0になったら消す
    if (GetHp() <= 0)
    {
        GameObject::Discard();
        Discard();
        DropSkillOrb(GetPos(), SKILLORB_SIZE_TYPE_BIG);
    }

    CollisionAction();

    GetAnimator()->SetIsAnim(true);

    float dt = Time::GetDeltaTime(); //初期化時のエラーを回避する


    //プレイヤー追従
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

    this->AddVel(GetVel());
}




void Enemy3::CollisionAction(void)
{
    std::list<Collider*> collisions = GetCollider()->GetCollision();

    for (auto collision : collisions)
    {
        OBJECT_TYPE type = collision->GetParent()->GetType();
        switch (type)
        {
        case OBJ_SOLID:
            if (GetVel().x != 0.0f)
                SetVel(Vector2(0.0f, GetVel().y));
            if (GetVel().y != 0.0f)
                SetVel(Vector2(GetVel().x, 0.0f));
            break;
        case OBJ_SPIKE:
            CollisionSpike();
            break;
        case OBJ_ATTACK:
            {
                PlayerAttack* attack = dynamic_cast<PlayerAttack*>(collision->GetParent());
                if(attack != nullptr)
                    SetHp(GetHp() - attack->GetDamage());
            }
            break;
        default:
            break;
        }
    }
}

//================================================================================
// トゲに当たった時のアクション
//================================================================================
void Enemy3::CollisionSpike(void)
{

    dir_ = GetVel().Normalize();
    if (abs(GetVel().x) < 0.1f && abs(GetVel().y < 0.1f))
    {
        if (GetVel().x != 0.0f)
            SetVel(Vector2(0.0f, GetVel().y));
        if (GetVel().y != 0.0f)
            SetVel(Vector2(GetVel().x, 0.0f));

        //knockback_end_がバグるのでそのまま
        knockback_start_ = GetPos();
        knockback_end_ = GetPos();
        return;
    }
    dir_ *= -1;	//反転させる

    HpDown(6);

    knockback_distance_ = SIZE_;

    knockback_start_ = GetPos();
    knockback_end_ = GetPos() - (dir_ * knockback_distance_);
}

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

