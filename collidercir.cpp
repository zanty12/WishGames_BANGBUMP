#include "collidercir.h"

ColliderCir::ColliderCir(GameObject* parent, float radius): Collider(CIRCLE, parent)
{
    cir_.a = parent->GetPos();
    radius_ = radius;
}

bool ColliderCir::Collide(Collider* other) const
{
    switch(other->GetType())
    {
    case CIRCLE:
        return Collider2D::Touch(cir_, dynamic_cast<ColliderCir*>(other)->GetCircle());
    case RECTANGLE:
        return Collider2D::Touch(cir_, dynamic_cast<ColliderRect*>(other)->GetRect());
        default:
            return false;
    }
}

void ColliderCir::Update()
{
    if(GetPos() != GetParent()->GetPos())
    {
        SetPos(GetParent()->GetPos());
        cir_.a = GetPos();
    }
}
