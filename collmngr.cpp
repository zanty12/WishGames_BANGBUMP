#include "collmngr.h"

void CollMngr::Update()
{
    //前のフレームでの衝突情報をリセット
    for(const auto collider : colliders_)
    {
        collider->SetCollision(std::list<Collider*>());
    }
    //衝突判定
    for(const auto collider : colliders_)
    {
        if(collider->GetIsMove())
            collider->Update();
    }
    //衝突処理
    for(auto collider : colliders_)
    {
        if(collider->GetIsMove())
        {
            for(auto other : colliders_)
            {
                if(collider == other)
                    continue;
                if(collider->Collide(other))
                {
                    collider->OnCollision(other);
                    other->OnCollision(collider);
                }
            }
        }
    }
}
