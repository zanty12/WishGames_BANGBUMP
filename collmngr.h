#pragma once
#include <list>

#include "collider.h"
#include "time.h"

class Camera;

class CollMngr
{
private:
    std::vector<std::list<Collider*>> static_colliders_; //�����Ȃ��R���C�_�[
    std::list<Collider*> dynamic_colliders_; //�����R���C�_�[

    float prev_time_ = 0.0f;

public:
    CollMngr()
    {
        prev_time_ = Time::GetCurrentTime();
    };

    ~CollMngr()
    {
        for (auto colliders : static_colliders_)
            for(auto collider: colliders)
                delete collider;
        static_colliders_.clear();
    }

    bool Add(Collider* collider);
    void Update();
    void Draw(Camera* camera);
    void CheckDiscard();
    void UpdateCollision(Collider* collider);

private:
    static void Collision(Collider* collider, Collider* other);
};
