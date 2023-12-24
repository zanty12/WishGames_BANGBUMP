#pragma once
#include "enemy.h"
#include "sprite.h"
#include "animator.h"

class Enemy3 : public Enemy
{
private:
    const int HP_MAX_ = 60;		//HPの上限
    int hp_;
    int atk_ = 15;
    const float spd_ = 96.0f * 2;
    Vector2 startPosition;
    bool cheakRange_Enemy_;
    bool cheakRange_Player_;

public:
    Enemy3() = delete;
    ~Enemy3() override = default;

    Enemy3(int x, int y, EnemyMngr* enemy_mngr) : Enemy(x, y, LoadTexture("data/texture/enemy3.png"), enemy_mngr)
    {
        startPosition = GetPos();
    }
    int GetAtk(void) const { return atk_; }
    int GetHp(void) const { return hp_; }		//HPのゲット
    void SetHp(int hp) { hp_ = hp; }			//HPのセット
    //HPの減少（ダメージが現在のHPを超える場合、HPは0になる）
    void HpDown(int damage) { damage <= hp_ ? hp_ -= damage : hp_ = 0; }

    void Update() override;

    void RangeEnemy(float a, float b, float c, float d);
    void RangePlayer(float a, float b, float c, float d);

/*private:
    void CellActions();*/
};
