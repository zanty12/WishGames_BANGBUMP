#include "gameobject.h"
#include "animator.h"
#include "collider.h"

GameObject::GameObject(Vector2 pos, float rot, int tex_number)
: pos_(pos), rot_(rot), tex_(tex_number)
{
    animator_ = new Animator(pos, scale_, 0, false, tex_number);
    animator_->SetParent(this);
    collider_ = nullptr;
}

void GameObject::Draw(void)
{
    //animator_->Draw();
}

void GameObject::Draw(Vector2 offset)
{
    //animator->Draw(offset);
}