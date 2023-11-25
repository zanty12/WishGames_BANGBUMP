#pragma once
#include "enemy.h"
#include "sprite.h"

class Koopa : public Enemy
{
public:
    Koopa() = delete;
    ~Koopa() override = default;
    Koopa(int x, int y) : Enemy(x, y)
    {
        SetTexNo(LoadTexture("data/texture/koopa.png"));
    }
    void Update() override;
    void Draw() override
    {
        DrawSprite(GetTexNo(), GetPos(), GetRot(), Vector2(size_,size_), Color(1.0f, 1.0f, 1.0f, 1.0f));
    };
};
