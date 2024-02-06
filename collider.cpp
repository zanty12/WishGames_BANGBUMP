#include "collider.h"

#include "gameobject.h"

Collider::Collider(COLLIDER_TYPE type, GameObject* parent,bool movable): type_(type)
{
    parent_ = parent;
    pos_ = parent->GetPos();
    is_movable_ = movable;
}

void Collider::Draw(int tex, Vector2 offset)
{
    const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
    const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
    DrawSprite(tex, (pos_-offset)*scale_x, parent_->GetRot(), parent_->GetScale()*scale_x, Color(0, 1, 0, 0.5f));
}
