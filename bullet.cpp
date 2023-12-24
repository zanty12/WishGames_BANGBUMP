/*==============================================================================
   ÉoÉåÉbÉgä«óù[bullet.cpp]
==============================================================================*/
#include "bullet.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"
#include "gamebase.h"


Bullet::Bullet(Vector2 pos, Vector2 vel) : Bullet(pos, LoadTexture("data/texture/bullet.png"), vel * spd_)
{

}

void Bullet::Update()
{
    std::list<Collider*> collisions = GetCollider()->GetCollision();
    for (auto collision : collisions)
    {
        OBJECT_TYPE type = collision->GetParent()->GetType();
    }
}
