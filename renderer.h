#pragma once
#include <list>

#include "animator.h"

class Animator;
class Camera;

class Renderer
{
private:
    std::list<Animator*> animators_;

public:
    Renderer()
    {
        animators_ = std::list<Animator*>();
    };
    ~Renderer()
    {
        for (auto animator : animators_)
        {
            delete animator;
        }
        animators_.clear();
    }
    void Update();
    void Draw();
    void Draw(Camera* camera);

    bool Add(Animator* animator)
    {
        if (animator == nullptr || animator->GetTexNo() < 0 || animator->GetPos().x < 0.0f || animator->GetPos().y < 0.0f)
            return false;
        animators_.push_back(animator);
        return true;
    }
    void CheckDiscard();
};
