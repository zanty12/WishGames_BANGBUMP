#include "collmngr.h"

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
        if(collider->GetIsMove())
            collider->Update();
    }
    //�Փˏ���
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
