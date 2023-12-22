#include "gameobject.h"
#include "animator.h"
#include "collider.h"
#include "colliderrect.h"

GameObject::GameObject(Vector2 pos, float rot, int tex_number)
: pos_(pos), rot_(rot), tex_(tex_number)
{
    animator_ = new Animator(pos_,scale_,tex_);
    collider_ = new ColliderRect(this);
}

void GameObject::Discard() { is_discard_ = true; animator_->Discard(); collider_->Discard(); }
