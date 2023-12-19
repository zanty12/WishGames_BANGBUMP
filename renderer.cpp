#include "renderer.h"
void Renderer::Update()
{
    for (auto animator : animators_)
    {
        /*if(animator->GetPos().x < 0.0f || animator->GetPos().y < 0.0f)
        {
            delete animator;
            animators_.remove(animator);
            animator--;
            continue;
        }*/ //FIXME: �Ԉ�����Ƃ��������
        if(animator->GetIsMovable() || animator->GetIsAnim())
            animator->Update();

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