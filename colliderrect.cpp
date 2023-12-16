#include "colliderrect.h"

ColliderRect::ColliderRect(GameObject* parent) : Collider(RECTANGLE, parent)
{
    const Vertex4 rect = Vertex4(Vector2(parent->GetPos().x - parent->GetScale().x / 2, parent->GetPos().y + parent->GetScale().y / 2),
                                 Vector2(parent->GetPos().x + parent->GetScale().x / 2, parent->GetPos().y + parent->GetScale().y / 2),
                                 Vector2(parent->GetPos().x + parent->GetScale().x / 2, parent->GetPos().y - parent->GetScale().y / 2),
                                 Vector2(parent->GetPos().x - parent->GetScale().x / 2, parent->GetPos().y - parent->GetScale().y / 2));
    rect_ = rect;
}

bool ColliderRect::Collide(Collider* other) const
{
    switch(other->GetType())
    {
    case CIRCLE:
        return Collider2D::Touch(rect_, dynamic_cast<ColliderCir*>(other)->GetCircle());
    case RECTANGLE:
        return Collider2D::Touch(rect_, dynamic_cast<ColliderRect*>(other)->GetRect());
    default:
        return false;
    }

}


void ColliderRect::Update()
{
    if(GetPos() != GetParent()->GetPos())
    {
        SetPos(GetParent()->GetPos());
        rect_.a = Vector2(GetPos().x - GetParent()->GetScale().x / 2, GetPos().y + GetParent()->GetScale().y / 2);
        rect_.b = Vector2(GetPos().x + GetParent()->GetScale().x / 2, GetPos().y + GetParent()->GetScale().y / 2);
        rect_.c = Vector2(GetPos().x + GetParent()->GetScale().x / 2, GetPos().y - GetParent()->GetScale().y / 2);
        rect_.d = Vector2(GetPos().x - GetParent()->GetScale().x / 2, GetPos().y - GetParent()->GetScale().y / 2);
    }
}
