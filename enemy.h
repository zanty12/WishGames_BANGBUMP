#pragma once
#include "movableobj.h"
#include "mapmngr.h"

class EnemyMngr;

class Enemy : public MovableObj
{
private:
    const int HP_MAX_ = 150;		//HP�̏��
    int hp_;
    EnemyMngr* enemy_mngr_;
    bool dead = false; //����ł��邩�ǂ���
public:
    Enemy() = delete;
    Enemy(int x, int y,int texNo,EnemyMngr* enemy_mngr) : MovableObj(Vector2((x + 0.5f) * SIZE_, (y + 0.5f) * SIZE_),0.0f,texNo,Vector2(0.0f,0.0f))
    {
        enemy_mngr_ = enemy_mngr;
    }
    virtual ~Enemy() = default;
    virtual void Update() = 0;
    virtual void Draw(Camera* camera);
    EnemyMngr* GetEnemyMngr() const { return enemy_mngr_; }
    bool IsDead() const { return dead; }
    void Die(){ this->dead = true; }
    //HP�̌����i�_���[�W�����݂�HP�𒴂���ꍇ�AHP��0�ɂȂ�j
    void HpDown(int damage) { damage <= hp_ ? hp_ -= damage : hp_ = 0; }
};
