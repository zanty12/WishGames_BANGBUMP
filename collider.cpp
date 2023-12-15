#include "collider.h"

Collider::Collider(GameObject* parent)
{
    parent_ = parent;
    pos_ = parent->GetPos();
}
