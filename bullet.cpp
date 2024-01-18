/*==============================================================================
   バレット管理[bullet.cpp]
==============================================================================*/
#include "bullet.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"
#include "gamebase.h"

#define RANGE (SIZE_ * 10.0f)                            //範囲

bool CheckBulletLength(Vector2 a, Vector2 b, float len);

Bullet::Bullet(Vector2 pos)
    : MovableObj(pos,0.0f, LoadTexture("data/texture/skillorb.png"), Vector2::Zero)
{
    SetPos(pos);
    SetType(OBJ_BULLET);
    SetAtk(10);
    startPosition = GetPos();

    // アニメーター設定
    SetTexenum(enemy2_attack);
    GetAnimator()->SetTexenum(GetTexenum());
    GetAnimator()->SetLoopAnim(ENEMY_2_ATTACK);
}

void Bullet::Update()
{
    AddVel(GetVel());
    CollisionAction();
    
    if (CheckBulletLength(GetPos(), startPosition, RANGE))
    {
        Die();
    }

}
void Bullet::CollisionAction(void)
{
    std::list<Collider*> collisions = GetCollider()->GetCollision();

    for (auto collision : collisions)
    {
        OBJECT_TYPE type = collision->GetParent()->GetType();
        switch (type)
        {
        case OBJ_SOLID:
            Die();
            break;
        case OBJ_SPIKE:
            Die();
            break;
        default:
            break;
        }
    }
}
bool CheckBulletLength(Vector2 a, Vector2 b, float len)
{

    if (Vector2::Distance(a, b) > len)
    {
        return true;
    }

    return false;
}
