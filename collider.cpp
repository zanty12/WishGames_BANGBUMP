#include "collider.h"

#include "gameobject.h"

Collider::Collider(COLLIDER_TYPE type, GameObject* parent): type_(type)
{
    parent_ = parent;
    pos_ = parent->GetPos();
}
