#include "gameobject.h"
#include "animator.h"
#include "collider.h"
#include "colliderrect.h"
#include "id.h"

GameObject::GameObject(Vector2 pos, float rot, int tex_number)
: pos_(pos), rot_(rot), tex_(tex_number)
{
    Vector2 world_coord = GetWorldCoord(pos_);
    id_ = ID::GenerateID(world_coord.y);
    animator_ = new Animator(this);
    collider_ = new ColliderRect(this,false);
}

//this is here only for movable objects
GameObject::GameObject(Vector2 pos, float rot, int tex_number,bool movable)
: pos_(pos), rot_(rot), tex_(tex_number)
{
    Vector2 world_coord = GetWorldCoord(pos_);
    //use y coordinate to hash id for sectioning
    id_ = ID::GenerateID(world_coord.y);
    animator_ = new Animator(this);
    collider_ = new ColliderRect(this,movable);
}


void GameObject::Discard()
{
    is_discard_ = true; 
    if (animator_) animator_->Discard();
    if (collider_) collider_->Discard();
}

void GameObject::DispUninit(void)
{
    color_ = Color(0, 0, 0, 0);
    animator_->SetIsAnim(false);
    animator_->SetColor(GetColor());
    SetType(OBJ_VOID);
}

Vector2 GameObject::GetWorldCoord(Vector2 pos)
{
    const int idx = std::floor((pos.x - GameObject::SIZE_ / 2) / GameObject::SIZE_);
    const int idy = std::floor((pos.y - GameObject::SIZE_ / 2) / GameObject::SIZE_);
    return Vector2(idx,idy);
}
