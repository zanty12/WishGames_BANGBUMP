#pragma once
#include <list>

#include "collider.h"

class CollMngr
{
private:
    std::list<Collider*> colliders_;
public:
    CollMngr() = default;
    ~CollMngr() = default;
    void AddCollider(Collider* collider) {colliders_.push_back(collider);}
    void Update();
};
