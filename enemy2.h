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
    float atk_time_;		// 攻撃時間

public:
    Enemy2() = delete;
    ~Enemy2() override = default;

    Enemy2(int x, int y, EnemyMngr* enemy_mngr) : Enemy(x, y, LoadTexture("data/texture/enemy2.png"), enemy_mngr)
    {
        startPosition = GetPos();
        SetScale(Vector2(SIZE_ * 2.5f, SIZE_ * 2.5f));
        SetAtk(10);
        SetHp(30);
        atk_time_ = 0;

        // アニメーター設定
        SetTexenum(enemy2_anim);
        GetAnimator()->SetTexenum(GetTexenum());
        GetAnimator()->SetLoopAnim(ENEMY_2_ANIM);
    }


    void Update() override;

/*private:
    void CellActions();*/
};

