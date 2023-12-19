#include "gameobject.h"
#include "animator.h"
#include "collider.h"

GameObject::GameObject(Vector2 pos, float rot, int tex_number)
: pos_(pos), rot_(rot), tex_(tex_number)
{
    animator_ = new Animator(this);
    animator_->SetParent(this);
    collider_ = nullptr;
}
