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
    ColliderRect(GameObject* parent,bool movable);
    void SetPos(Vector2 pos);
    Vertex4 GetRect() const { return rect_; }
    bool Collide(Collider* other) override;
    void Update() override;
    void CollisionInteract() override;
    void CollisionSolid(Collider* other) override;
    void CollisionPen(Collider* other) override;

private:
    static bool CheckIntersect(Vertex4 rect, Vertex4 other);
};
