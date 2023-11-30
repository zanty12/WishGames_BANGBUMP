#pragma once
#include "enemy.h"
#include "sprite.h"

class Koopa : public Enemy
{
private:
    const float x_spd_ = 96.0f;
    const float y_spd_ = 0.5f;
public:
    Koopa() = delete;
    ~Koopa() override = default;
    Koopa(int x, int y,EnemyMngr* enemy_mngr) : Enemy(x, y,enemy_mngr)
    {
        SetTexNo(LoadTexture("data/texture/koopa.png"));
    }
    void Update() override;
    void Draw() override
    {
        DrawSprite(GetTexNo(), GetPos(), GetRot(), Vector2(size_,size_), Color(1.0f, 1.0f, 1.0f, 1.0f));
    }
private:
    void CellActions();
};
