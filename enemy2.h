#pragma once
#include "enemy.h"
#include "sprite.h"
#include "animator.h"
#include "projectilemngr.h"

class Bullet;

class Enemy2 : public Enemy
{
private:
    Vector2 startPosition;
    Vector2 bullet_vel_ = Vector2(0.0f, 0.0f);
    float atk_time_;		// çUåÇéûä‘

public:
    Enemy2() = delete;
    ~Enemy2() override = default;

    Enemy2(int x, int y, EnemyMngr* enemy_mngr) : Enemy(x, y, LoadTexture("data/texture/enemy2.png"), enemy_mngr)
    {
        SetScale(Vector2(SIZE_ * 4, SIZE_ * 4));
        SetAtk(10);
        SetHp(30);
        atk_time_ = 0;
    }


    void Update() override;

/*private:
    void CellActions();*/
};

