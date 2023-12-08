#pragma once
#include "enemy.h"
#include "sprite.h"
#include "animator.h"

class Enemy2 : public Enemy
{
private:
    //const float x_spd_ = 96.0f;
    //const float y_spd_ = 0.5f;

public:
    Enemy2() = delete;
    ~Enemy2() override = default;

    Enemy2(int x, int y, EnemyMngr* enemy_mngr) : Enemy(x, y, LoadTexture("data/texture/enemy2.png"), enemy_mngr)
    {}

    void Update() override;


private:
    void CellActions();
};
