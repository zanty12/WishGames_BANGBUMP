#pragma once
#include "enemy.h"
#include "sprite.h"
#include "animator.h"

class Bullet;

class Enemy2 : public Enemy
{
private:
    const int HP_MAX_ = 30;		//HP�̏��
    int hp_;
    Vector2 startPosition;
    Vector2 bullet_vel_ = Vector2(0.0f, 0.0f);
    SKILLORB_ATTRIBUTE_DESC drop;

public:
    Enemy2() = delete;
    ~Enemy2() override = default;

    Enemy2(int x, int y, EnemyMngr* enemy_mngr) : Enemy(x, y, LoadTexture("data/texture/enemy2.png"), enemy_mngr)
    {
        SetScale(Vector2(SIZE_ * 4, SIZE_ * 4));
        SetAtk(10);
    }

    Bullet* CreatBullet(void);

    int GetHp(void) const { return hp_; }		//HP�̃Q�b�g
    void SetHp(int hp) { hp_ = hp; }			//HP�̃Z�b�g
    //HP�̌����i�_���[�W�����݂�HP�𒴂���ꍇ�AHP��0�ɂȂ�j
    void HpDown(int damage) { damage <= hp_ ? hp_ -= damage : hp_ = 0; }

    void Update() override;

    SkillOrb* DropSkillOrb() override;

/*private:
    void CellActions();*/
};

