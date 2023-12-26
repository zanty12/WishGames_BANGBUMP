#include "collmngr.h"

#include "gamebase.h"
#include "time.h"

bool CollMngr::Add(Collider* collider)
{
    if (collider == nullptr)
        return false;
    colliders_.push_back(collider);
    return true;
}

void CollMngr::Update()
{
    //�O�̃t���[���ł̏Փˏ������Z�b�g
    for (const auto collider : colliders_)
    {
        collider->SetCollision(std::list<Collider*>());
        collider->SetLoD(GameBase::UpdateLoD(collider->GetPos()));
    }

    //�j������
    colliders_.remove_if(
        [](Collider* collider)
        {
            if (collider->GetDiscard())
            {
                delete collider;
                return true;
            }
            return false;
        }
    );

    //���ʒu�X�V
    for (auto it : colliders_)
    {
        if (it->GetIsMovable() && !it->GetLoD())
            it->Update();

        else if (it->GetIsMovable() && it->GetLoD() && Time::GetDeltaTime(prev_time_) >= 1000.0f / 30.0f)
        {
            it->Update();
            //prev_time_ = time_;
        }
    }

    //�Փ�
    for (const auto collider : colliders_)
    {
        if (collider->GetIsMovable() && !collider->GetLoD())
        {
            for (const auto other : colliders_)
            {
                if (collider == other)
                    continue;
                if (collider->Collide(other))
                {
                    collider->AddCollision(other);
                    if (other->GetIsMovable())
                        other->AddCollision(collider);
                }
            }
        }
        else if(collider->GetIsMovable() && collider->GetLoD() && Time::GetDeltaTime(prev_time_) >= 1000.0f / 30.0f)
        {
            for (const auto other : colliders_)
            {
                if (collider == other)
                    continue;
                if (collider->Collide(other))
                {
                    collider->AddCollision(other);
                    if (other->GetIsMovable())
                        other->AddCollision(collider);
                }
            }
        }
    }

    for (const auto collider : colliders_)
    {
        if (collider->GetIsMovable() && !collider->GetLoD())
            collider->CollisionInteract();
        else if(collider->GetIsMovable() && collider->GetLoD() && Time::GetDeltaTime(prev_time_) >= 1000.0f / 30.0f)
        {
            collider->CollisionInteract();
        }
    }
    if(Time::GetDeltaTime(prev_time_) >= 1 / 30.0f)
        prev_time_ = Time::GetCurrentTime();
}
