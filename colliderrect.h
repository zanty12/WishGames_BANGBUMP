#pragma once
#include "collider.h"
#include "collidercir.h"

using namespace PHYSICS;
class ColliderRect : public Collider
{
private:
    Vertex4 rect_;

public:
    ColliderRect() = delete;
    ColliderRect(GameObject* parent);
    Vertex4 GetRect() const { return rect_; }
    bool Collide(Collider* other) const override;
    void Update() override;
};
