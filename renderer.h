#pragma once
#include <list>

#include "animator.h"
class Animator;

class Renderer
{
private:
    std::list<Animator*> animators_;

public:
    Renderer() = default;
    ~Renderer() = default;
    void Update();
    void Draw();
    void Draw(Vector2 offset);

    bool Add(Animator* animator)
    {
        animators_.push_back(animator);
        return true;
    }
};
