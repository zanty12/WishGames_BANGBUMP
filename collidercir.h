#pragma once
#include "collider.h"
#include "colliderrect.h"

using namespace PHYSICS;
class ColliderCir: public Collider
{
private:
    Vertex1 cir_;
    float radius_;

    Animator* animator_;
public:
    ColliderCir() = delete;
    ColliderCir(GameObject* parent, float radius);
    Vertex1 GetCircle() const { return cir_; }
    float GetRadius() const { return radius_; }
    bool Collide(Collider* other) override;
    void Update() override;

    void CollisionInteract() override;
    void CollisionSolid(Collider* other) override;
    void CollisionPen(Collider* other) override;
};
