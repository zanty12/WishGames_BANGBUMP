#include "renderer.h"
#include "camera.h"


void Renderer::InitRenderer()
{
    ImageDataDictionary::InitDictionary();  //ImageDataDictionary‚ÌŽ«‘“o˜^‚ð‚µ‚Ä‚©‚ç
    Animator::InitDictionary();             //Animator‚ÌŽ«‘“o˜^‚ðs‚¤iImageDataDictionary‚ÌŽ«‘‚ðŽg‚¤‚½‚ßj
}

void Renderer::Update()
{
    CheckDiscard();

    animators_.sort(
        [](Animator* a, Animator* b) {
            return a->GetDrawPriority() < b->GetDrawPriority();
        }
    );

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
            if (!animator)
            {
                delete animator;
                return true;
            }
            if (animator->GetDiscard())
            {
                delete animator;
                animator = nullptr;
                return true;
            }
            return false;
        }
    );
}
