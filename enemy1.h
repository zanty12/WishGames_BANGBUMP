#pragma once
#include "enemy.h"
#include "sprite.h"
#include "animator.h"
#include "time.h"

class Enemy1 : public Enemy
{
private:
    const float x_spd_ = 96.0f;
    const float y_spd_ = 0.5f;
    Vector2 startPosition;

public:
    Enemy1() = delete;
    ~Enemy1() override = default;

    Enemy1(int x, int y, EnemyMngr* enemy_mngr) : Enemy(x, y,LoadTexture("data/texture/enemy1.png"), enemy_mngr)
    { 
        startPosition = GetPos();
        float dt = Time::GetDeltaTime() < 1 ? Time::GetDeltaTime() : 0.0f; //初期化時のエラーを回避する
        SetVel(Vector2(-x_spd_ * dt, GetVel().y));
    }

    void Update() override;


private:
    void CellActions();
};
