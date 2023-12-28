#include "collmngr.h"

#include <mutex>
#include <thread>

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
    //破棄する
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

    //前のフレームでの衝突情報をリセット
    for (const auto collider : colliders_)
    {
        collider->SetCollision(std::list<Collider*>());
        collider->SetLoD(GameBase::UpdateLoD(collider->GetPos()));
    }
    //一回位置更新
    for (auto it : colliders_)
    {
        if (it->GetIsMovable() && !it->GetLoD())
            it->Update();

        else if (it->GetIsMovable() && it->GetLoD() && Time::GetDeltaTime(prev_time_) >= 1000.0f / 25.0f)
        {
            it->Update();
        }
    }

    //衝突
    for (const auto collider : colliders_)
    {
        if (collider->GetIsMovable() && !collider->GetLoD())
        {
            for(const auto other : colliders_)
            {
                if (collider == other)
                    continue;
                if (collider->GetLoD() != other->GetLoD())
                    continue;
                Collision(collider, other);
            }
        }
        else if (collider->GetIsMovable() && collider->GetLoD() && Time::GetDeltaTime(prev_time_) >= 1000.0f / 25.0f)
        {
            for (const auto other : colliders_)
            {
                if (collider == other)
                    continue;
                if (collider->GetLoD() != other->GetLoD())
                    continue;
                Collision(collider, other);
            }
        }
    }

    for (const auto collider : colliders_)
    {
        if (collider->GetIsMovable() && !collider->GetLoD())
            collider->CollisionInteract();
        else if (collider->GetIsMovable() && collider->GetLoD() && Time::GetDeltaTime(prev_time_) >= 1000.0f / 25.0f)
        {
            collider->CollisionInteract();
        }
    }

    if (Time::GetDeltaTime(prev_time_) >= 1000.0f / 25.0f)
        prev_time_ = Time::GetCurrentTime();
}

void CollMngr::Collision(Collider* collider, Collider* other)
{
    if (collider == other)
        return;
    if (collider->Collide(other))
    {
        collider->AddCollision(other);
        if (other->GetIsMovable())
            other->AddCollision(collider);
    }
}
