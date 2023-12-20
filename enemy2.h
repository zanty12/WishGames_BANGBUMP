#pragma once
#include "enemy.h"
#include "sprite.h"
#include "animator.h"

class Enemy2 : public Enemy
{
private:
    const int HP_MAX_ = 30;		//HPÇÃè„å¿
    int hp_;
    Vector2 startPosition;

public:
    Enemy2() = delete;
    ~Enemy2() override = default;

    Enemy2(int x, int y, EnemyMngr* enemy_mngr) : Enemy(x, y, LoadTexture("data/texture/enemy2.png"), enemy_mngr)
    {}

    void Update() override;
    void Spawn(int x, int y, int type);

/*private:
    void CellActions();*/
};

