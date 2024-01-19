#include "enemy2.h"
#include "bullet.h"
#include "Cell.h"
#include "MapMngr.h"
#include "playerattack.h"
#include "lib/collider2d.h"
#include "time.h"

#define RANGE (SIZE_ * 10.0f)                            //îÕàÕ


bool CheckEnemy2Length(Vector2 a, Vector2 b, float len);


void Enemy2::Update()
{
    //HPÇ™0Ç…Ç»Ç¡ÇΩÇÁè¡Ç∑
    if (GetHp() <= 0)
    {
        GameObject::Discard();
        Die();
        DropSkillOrb(GetPos(), SKILLORB_SIZE_TYPE_MID);
    }

    float dt = Time::GetDeltaTime(); //èâä˙âªéûÇÃÉGÉâÅ[ÇâÒîÇ∑ÇÈ
    atk_time_ += Time::GetDeltaTime();

    std::list<Collider*> collisions = GetCollider()->GetCollision();
    for (auto collision : collisions)
    {
        OBJECT_TYPE type = collision->GetParent()->GetType();
        switch (type)
        {
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

    GetAnimator()->SetIsAnim(true);

    std::list<Player*> players = GetEnemyMngr()->GetMapMngr()->GetGame()->GetPlayers();

    float Spos_now = 0.0f;
    float Spos_old = 10000.0f;

    Player* close_player = nullptr;
    Bullet* bullet = nullptr;

    for (auto player : players)
    {
        if (CheckEnemy2Length(startPosition, player->GetPos(), RANGE))
        {
            Spos_now = Vector2::Distance(GetPos(), player->GetPos());
            if (Spos_now < Spos_old)
            {
                close_player = player;
                Spos_old = Spos_now;
            }
        }
    }

    if (close_player != nullptr)
    {
        if (atk_time_ > 2.0f)
        {
            atk_time_ = 0;
            Vector2 bullet_pos;
            bullet = new Bullet(GetPos());
            Vector2 distance = close_player->GetPos() - startPosition;
            bullet_pos = distance.Normalize();
            bullet->SetVel(distance.Normalize() * (96.0f * 2) * dt);
            bullet->SetPos(GetPos() + (bullet_pos * SIZE_ * 2.5f));
            GetEnemyMngr()->GetMapMngr()->GetGame()->GetProjectileMngr()->Add(bullet);
        }
    }
}


bool CheckEnemy2Length(Vector2 a, Vector2 b, float len)
{
    if (Vector2::Distance(a, b) < len)
    {
        return true;
    }

    return false;
}
