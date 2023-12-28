#pragma once
#include <list>

#include "collider.h"
#include "time.h"

class CollMngr
{
private:
    std::list<Collider*> colliders_;
    float prev_time_ = 0.0f;

public:
    CollMngr()
    {
        colliders_ = std::list<Collider*>();
        prev_time_ = Time::GetCurrentTime();
    };

    ~CollMngr()
    {
        for (auto collider : colliders_)
            delete collider;
        colliders_.clear();
    }

    bool Add(Collider* collider);
    void Update();

private:
    static void Collision(Collider* collider, Collider* other);
};
