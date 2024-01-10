#include "collmngr.h"

#include <mutex>
#include <thread>

#include "gamebase.h"
#include "id.h"
#include "time.h"

bool CollMngr::Add(Collider* collider)
{
    if (collider == nullptr)
        return false;
    if (collider->GetIsMovable())
    {
        dynamic_colliders_.push_back(collider);
        return true;
    }
    //Get index from first 2 digits of id
    std::string index = collider->GetParent()->GetID();
    index = index.substr(0, 2);
    int i = std::stoi(index);
    //Add to static colliders
    if (i >= static_colliders_.size())
        static_colliders_.resize(i + 1, std::list<Collider*>());
    static_colliders_[i].push_back(collider);
    return true;
}

void CollMngr::Update()
{
    //破棄する
    CheckDiscard();

    //前のフレームでの衝突情報をリセット
    for (const auto collider : dynamic_colliders_)
    {
        collider->SetCollision(std::list<Collider*>());
        collider->SetLoD(GameBase::UpdateLoD(collider->GetPos()));
    }
    //一回位置更新
    for (auto it : dynamic_colliders_)
    {
        if (!it->GetLoD())
            it->Update();

        else if (it->GetLoD() && Time::GetDeltaTime(prev_time_) >= 1000.0f / 25.0f)
        {
            it->Update();
        }
    }

    //衝突
    for (const auto collider : dynamic_colliders_)
    {
        if (!collider->GetLoD())
        {
            //動くコライダーと衝突
            for (const auto other : dynamic_colliders_)
            {
                if (collider == other)
                    continue;
                if (collider->GetLoD() != other->GetLoD())
                    continue;
                Collision(collider, other);
            }
            //動かないコライダーと衝突
            Vector2 world_pos = GameObject::GetWorldCoord(collider->GetPos());
            const int hash = ID::Hash(world_pos.y);
            if (hash > static_colliders_.size())
                continue;
            for (const auto other : static_colliders_[hash])
            {
                if (other != nullptr)
                    Collision(collider, other);
            }
            //check other two sections just to be sure
            if (hash - 1 >= 0)
            {
                for (const auto other : static_colliders_[hash - 1])
                {
                    if (other != nullptr)
                        Collision(collider, other);
                }
            }
            if (hash + 1 < static_colliders_.size())
            {
                for (const auto other : static_colliders_[hash + 1])
                {
                    if (other != nullptr)
                        Collision(collider, other);
                }
            }
        }
        else if (collider->GetIsMovable() && collider->GetLoD() && Time::GetDeltaTime(prev_time_) >= 1000.0f / 25.0f)
        {
            for (const auto other : dynamic_colliders_)
            {
                if (collider == other)
                    continue;
                if (collider->GetLoD() != other->GetLoD())
                    continue;
                Collision(collider, other);
            }
        }
    }

    for (const auto collider : dynamic_colliders_)
    {
        if (!collider->GetLoD())
            collider->CollisionInteract();
        else if (collider->GetLoD() && Time::GetDeltaTime(prev_time_) >= 1000.0f / 25.0f)
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

void CollMngr::CheckDiscard()
{
    dynamic_colliders_.remove_if(
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
    for (auto& colliders : static_colliders_)
    {
        colliders.remove_if(
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
    }
}
