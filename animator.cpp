#include "animator.h"
#include "gameobject.h"
#include "game.h"
#include "renderer.h"

Animator::Animator(Vector2 pos, Vector2 scale, int fps, bool isAnim, int texNo)
        : pos_(pos), scale_(scale), fps_(fps), isAnim_(isAnim), texNo_(texNo)
{
    Game::GetRenderer()->Add(this);
}

void Animator::Update(void)
{
    if(isAnim_)
    {
        pos_ = parent_->GetPos();
    }
}
