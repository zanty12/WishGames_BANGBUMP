#include "animator.h"
#include "gameobject.h"
#include "game.h"
#include "renderer.h"

Animator::Animator(Vector2 pos, Vector2 scale, int fps, bool isAnim, int texNo)
        : pos_(pos), scale_(scale), fps_(fps), isAnim_(isAnim), texNo_(texNo)
{
    if(!Game::GetRenderer()->Add(this))
        std::cout<<"error creating animator for obj at "<<pos_.x<<", "<<pos_.y<<std::endl;
}

void Animator::Update(void)
{
    if(isMovable_)
    {
        pos_ = parent_->GetPos();
    }
}
