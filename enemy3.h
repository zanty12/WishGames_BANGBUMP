#pragma once
#include "enemy.h"
#include "sprite.h"
#include "animator.h"

class Enemy3 : public Enemy
{
private:
    const int HP_MAX_ = 60;		//HPの上限
    int hp_;
    const float x_spd_ = 96.0f * 2;
    const float y_spd_ = 96.0f * 2;
    Vector2 startPosition;

public:
    Enemy3() = delete;
    ~Enemy3() override = default;

    Enemy3(int x, int y, EnemyMngr* enemy_mngr) : Enemy(x, y, LoadTexture("data/texture/enemy3.png"), enemy_mngr)
    {
        startPosition = GetPos();
    }

    int GetHp(void) const { return hp_; }		//HPのゲット
    void SetHp(int hp) { hp_ = hp; }			//HPのセット
    //HPの減少（ダメージが現在のHPを超える場合、HPは0になる）
    void HpDown(int damage) { damage <= hp_ ? hp_ -= damage : hp_ = 0; }

    void Update() override;


private:
    void CellActions();
};
