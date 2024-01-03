/*==============================================================================
   ÉoÉåÉbÉgä«óù[bullet.cpp]
==============================================================================*/
#include "bullet.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"
#include "gamebase.h"


Bullet::Bullet(Vector2 pos)
    : MovableObj(pos,0.0f, LoadTexture("data/texture/skillorb.png"), Vector2::Zero)
{
    SetPos(pos);
    SetType(OBJ_BULLET);
    SetAtk(10);
    SetSpd(96.0f * 2);
}

void Bullet::Update()
{
    CollisionAction();
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
