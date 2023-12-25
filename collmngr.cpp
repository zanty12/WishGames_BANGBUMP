#include "collmngr.h"

bool CollMngr::Add(Collider* collider)
{
    if(collider == nullptr)
        return false;
    colliders_.push_back(collider);
    return true;
}

void CollMngr::Update()
{
    //前のフレームでの衝突情報をリセット
    for(const auto collider : colliders_)
    {
        collider->SetCollision(std::list<Collider*>());
    }

    //一回位置更新
    for (auto it : colliders_)
    {
        if (it->GetIsMovable())
            it->Update();
    }

    //破棄する
    colliders_.remove_if(
        [](Collider* collider) {
            if (collider->GetDiscard()) {
                delete collider;
                return true;
            }
            return false;
        }
    );


    //衝突
    for(const auto collider : colliders_)
    {
        if(collider->GetIsMovable())
        {
            for(const auto other : colliders_)
            {
                if(collider == other)
                    continue;
                if(collider->Collide(other))
                {
                    collider->AddCollision(other);
                    if(other->GetIsMovable())
                        other->AddCollision(collider);
                }
            }
        }
    }
    for(const auto collider : colliders_)
    {
        if(collider->GetIsMovable())
            collider->CollisionInteract();
    }
}
