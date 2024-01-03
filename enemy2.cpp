#include "enemy2.h"
#include "bullet.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"

#define RANGE (SIZE_ * 10.0f)                            //îÕàÕ


bool CheckEnemy2Length(Vector2 a, Vector2 b, float len);

Bullet* Enemy2::CreatBullet(void)
{
    return new Bullet(startPosition);
}

void Enemy2::Update()
{
    //HPÇ™0Ç…Ç»Ç¡ÇΩÇÁè¡Ç∑
    if (GetHp() <= 0)
    {
        GameObject::Discard();
        Die();
    }

    float dt = Time::GetDeltaTime(); //èâä˙âªéûÇÃÉGÉâÅ[ÇâÒîÇ∑ÇÈ

    std::list<Collider*> collisions = GetCollider()->GetCollision();
    for (auto collision : collisions)
    {
        OBJECT_TYPE type = collision->GetParent()->GetType();
    }

    GetAnimator()->SetIsAnim(true);

    std::list<Player*> players = GetEnemyMngr()->GetMapMngr()->GetGame()->GetPlayers();

    float Spos_now = 0.0f;
    float Spos_old = 0.0f;

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
        Vector2 distance = close_player->GetPos() - startPosition;
        CreatBullet();
        bullet->SetVel(distance.Normalize() * bullet->GetSpd() * dt);
    }


}

SkillOrb* Enemy2::DropSkillOrb()
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

    return new SkillOrb(GetPos(), drop, SKILLORB_SIZE_DESC::Mid());
}

bool CheckEnemy2Length(Vector2 a, Vector2 b, float len)
{

    if (Vector2::Distance(a, b) < len)
    {
        return true;
    }

    return false;
}

