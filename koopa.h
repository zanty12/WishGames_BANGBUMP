#pragma once
#include "enemy.h"
#include "sprite.h"
#include "animator.h"

class Koopa : public Enemy
{
private:
    const float x_spd_ = 96.0f;
    const float y_spd_ = 0.5f;

public:
    Koopa() = delete;
    ~Koopa() override = default;

    Koopa(int x, int y, EnemyMngr* enemy_mngr) : Enemy(x, y,LoadTexture("data/texture/koopa.png"), enemy_mngr)
    {
        GetAnimator()->SetIsAnim(true);
    }

    void Update() override;

private:
    void CellActions();
};
