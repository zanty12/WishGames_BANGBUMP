/*==============================================================================
   ƒoƒŒƒbƒgŠÇ—[bullet.cpp]
==============================================================================*/
#include "bullet.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"

void Bullet::Update()
{
    std::list<Collider*> collisions = GetCollider()->GetCollision();
    for (auto collision : collisions)
    {
        OBJECT_TYPE type = collision->GetParent()->GetType();

        //ÀÛ‚Ìˆ—
        if (type == OBJ_PLAYER)
        {
            Player* player = dynamic_cast<Player*> (collision->GetParent());
            player->HpDown(15);//š‰¼š
        }
    }

}
