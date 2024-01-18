#pragma once
#include "enemy.h"
#include "sprite.h"
#include "animator.h"
#include "time.h"

class Enemy1 : public Enemy
{
private:
    Vector2 dir_;		//向き
    const float x_spd_ = 96.0f;
    const float y_spd_ = 0.5f;
    Vector2 startPosition;
    float knockback_distance_;	//ノックバックする距離
    Vector2 knockback_start_;	//ノックバックの初めのポジション
    Vector2 knockback_end_;		//ノックバックの終わりのポジション


public:
    Enemy1() = delete;
    ~Enemy1() override = default;
    
    Enemy1(int x, int y, EnemyMngr* enemy_mngr) : Enemy(x, y,LoadTexture("data/texture/enemy1.png"), enemy_mngr)
    { 
        startPosition = GetPos();
        float dt = Time::GetDeltaTime() < 1 ? Time::GetDeltaTime() : 0.0f; //初期化時のエラーを回避する
        SetVel(Vector2(-x_spd_ * dt, GetVel().y));
        SetScale(Vector2(SIZE_ * 2, SIZE_ * 2));
        SetAtk(5);
        SetHp(20);

        // アニメーター設定
        SetTexenum(enemy1_anim);
        GetAnimator()->SetTexenum(GetTexenum());
        GetAnimator()->SetLoopAnim(ENEMY_1_ANIM);
    }

    void Update() override;


private:

    void CollisionAction(void);

    void CollisionSpike(void);

};
