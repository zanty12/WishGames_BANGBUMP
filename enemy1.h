#pragma once
#include "enemy.h"
#include "sprite.h"
#include "animator.h"
#include "time.h"

class Enemy1 : public Enemy
{
private:
    const int HP_MAX_ = 20;		//HPの上限
    int hp_;
    Vector2 dir_;		//向き
    const float x_spd_ = 96.0f;
    const float y_spd_ = 0.5f;
    Vector2 startPosition;
    int knock_back_dir_;	//トゲに衝突した方向

public:
    Enemy1() = delete;
    ~Enemy1() override = default;
    
    Enemy1(int x, int y, EnemyMngr* enemy_mngr) : Enemy(x, y,LoadTexture("data/texture/enemy1.png"), enemy_mngr/*, hp_(HP_MAX_)*/)
    { 
        startPosition = GetPos();
        float dt = Time::GetDeltaTime() < 1 ? Time::GetDeltaTime() : 0.0f; //初期化時のエラーを回避する
        SetVel(Vector2(-x_spd_ * dt, GetVel().y));
    }
    int GetHp(void) const { return hp_; }		//HPのゲット
    void SetHp(int hp) { hp_ = hp; }			//HPのセット
    //HPの減少（ダメージが現在のHPを超える場合、HPは0になる）
    void HpDown(int damage) { damage <= hp_ ? hp_ -= damage : hp_ = 0; }

    void Update() override;

/*private:
    void CellAction();
    void CellActionSpike();*/
};
