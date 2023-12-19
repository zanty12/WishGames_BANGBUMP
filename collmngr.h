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
    bool Add(Collider* collider);
    void Update();
};
