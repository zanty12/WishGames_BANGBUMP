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
    //�O�̃t���[���ł̏Փˏ������Z�b�g
    for(const auto collider : colliders_)
    {
        collider->SetCollision(std::list<Collider*>());
    }
    //���ʒu�X�V
    for(auto it = colliders_.begin(); it != colliders_.end();)
    {
        if((*it)->GetDiscard())
        {
            delete *it;
            it = colliders_.erase(it);
        }
        else
        {
            if((*it)->GetIsMovable())
                (*it)->Update();
            ++it;
        }
    }

    //�Փ�
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
