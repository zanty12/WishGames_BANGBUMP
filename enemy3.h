#pragma once
#include "enemy.h"
#include "sprite.h"
#include "animator.h"

class Enemy3 : public Enemy
{
private:
    Vector2 dir_;		//向き
    const float spd_ = 96.0f * 2;
    Vector2 startPosition;
    float knockback_distance_;	//ノックバックする距離
    Vector2 knockback_start_;	//ノックバックの初めのポジション
    Vector2 knockback_end_;		//ノックバックの終わりのポジション
    bool cheakRange_Enemy_;
    bool cheakRange_Player_;
    SKILLORB_ATTRIBUTE_DESC drop;

public:
    Enemy3() = delete;
    ~Enemy3() override = default;

    Enemy3(int x, int y, EnemyMngr* enemy_mngr) : Enemy(x, y, LoadTexture("data/texture/enemy3.png"), enemy_mngr)
    {
        startPosition = GetPos();
        SetScale(Vector2(SIZE_ * 5, SIZE_ * 5));
        SetAtk(15);
        SetHp(60);
    }

    void Update() override;

    SkillOrb* DropSkillOrb() override;

    void RangeEnemy(float a, float b, float c, float d);
    void RangePlayer(float a, float b, float c, float d);

private:

    void CollisionAction(void);

    void CollisionSpike(void);
};
