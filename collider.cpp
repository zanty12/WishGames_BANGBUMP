#include "collider.h"

#include "gameobject.h"

Collider::Collider(COLLIDER_TYPE type, GameObject* parent,bool movable): type_(type)
{
    parent_ = parent;
    pos_ = parent->GetPos();
    is_movable_ = movable;
}
