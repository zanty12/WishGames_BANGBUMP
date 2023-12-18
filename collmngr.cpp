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
    //�Փ˔���
    for(const auto collider : colliders_)
    {
        if(collider->GetIsMovable())
            collider->Update();
    }
    //�Փˏ���
    for(auto collider : colliders_)
    {
        if(collider->GetIsMovable())
        {
            for(auto other : colliders_)
            {
                if(collider == other)
                    continue;
                if(collider->Collide(other))
                {
                    collider->OnCollision(other);
                    if(other->GetIsMovable())
                        other->OnCollision(collider);
                }
            }
        }
    }
}
