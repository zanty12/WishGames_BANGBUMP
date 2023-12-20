#include "renderer.h"
void Renderer::Update()
{
    for(auto it = animators_.begin(); it != animators_.end();)
    {
        if((*it)->GetDiscard())
        {
            delete *it;
            it = animators_.erase(it);
        }
        else
        {
            if((*it)->GetIsMovable()||(*it)->GetIsAnim())
                (*it)->Update();
            ++it;
        }
    }
}
void Renderer::Draw() { for (auto animator : animators_)animator->Draw(); }

void Renderer::Draw(Vector2 offset)
{
    for (std::list<Animator*>::iterator it = animators_.begin(); it != animators_.end(); ++it)
    {
        if(*it != nullptr)
            (*it)->Draw(offset);
    }
}