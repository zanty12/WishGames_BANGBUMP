#pragma once
#include "movableobj.h"
#include "texture.h"

class Enemy : public MovableObj
{
public:
    Enemy() = delete;
    Enemy(int x, int y) : MovableObj(Vector2((x + 0.5) * size_, (y + 0.5) * size_),0.0f,0,Vector2(0.0f,0.0f)){}
    virtual ~Enemy() = default;
    virtual void Update() = 0;
    virtual void Draw() = 0;
};
