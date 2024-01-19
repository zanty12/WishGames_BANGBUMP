#include "renderer.h"
#include "camera.h"

void Renderer::Update()
{
    CheckDiscard();

    for (auto it = animators_.begin(); it != animators_.end();)
    {
        if ((*it)->GetIsMovable() || (*it)->GetIsAnim())
            (*it)->Update();
        ++it;
    }
}

void Renderer::Draw() { for (auto animator : animators_)animator->Draw(); }

void Renderer::Draw(Camera* camera)
{
    Vector2 offset = camera->GetCameraOffset();
    for (std::list<Animator*>::iterator it = animators_.begin(); it != animators_.end(); ++it)
    {
        if (*it != nullptr && camera->InCamera((*it)->GetPos()))
            (*it)->Draw(offset);
    }
}

void Renderer::CheckDiscard()
{
    animators_.remove_if(
        [](Animator* animator)
        {
            if (animator->GetDiscard())
            {
                delete animator;
                return true;
            }
            return false;
        }
    );
}
