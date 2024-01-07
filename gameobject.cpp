#include "gameobject.h"
#include "animator.h"
#include "collider.h"
#include "colliderrect.h"
#include "id.h"

GameObject::GameObject(Vector2 pos, float rot, int tex_number)
: pos_(pos), rot_(rot), tex_(tex_number)
{
    animator_ = new Animator(this);
    collider_ = new ColliderRect(this);
    Vector2 world_coord = GetWorldCoord(pos_);
    id_ = ID::GenerateID(world_coord.x + world_coord.y);
}

void GameObject::Discard() { is_discard_ = true; animator_->Discard(); collider_->Discard(); }

Vector2 GameObject::GetWorldCoord(Vector2 pos)
{
    const int idx = std::floor((pos.x - GameObject::SIZE_ / 2) / GameObject::SIZE_);
    const int idy = std::floor((pos.y - GameObject::SIZE_ / 2) / GameObject::SIZE_);
    return Vector2(idx,idy);
}
