#pragma once
#include "enemy.h"
#include "sprite.h"
#include "animator.h"

class Enemy2 : public Enemy
{
private:
    const int HP_MAX_ = 30;		//HP�̏��
    int hp_;
    int atk_ = 10;
    Vector2 startPosition;

public:
    Enemy2() = delete;
    ~Enemy2() override = default;

    Enemy2(int x, int y, EnemyMngr* enemy_mngr) : Enemy(x, y, LoadTexture("data/texture/enemy2.png"), enemy_mngr)
    {}

    int GetAtk(void) const { return atk_; }
    int GetHp(void) const { return hp_; }		//HP�̃Q�b�g
    void SetHp(int hp) { hp_ = hp; }			//HP�̃Z�b�g
    //HP�̌����i�_���[�W�����݂�HP�𒴂���ꍇ�AHP��0�ɂȂ�j
    void HpDown(int damage) { damage <= hp_ ? hp_ -= damage : hp_ = 0; }

    void Update() override;
    void Spawn(int x, int y, int type);

/*private:
    void CellActions();*/
};

