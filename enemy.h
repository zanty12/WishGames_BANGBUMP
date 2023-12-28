#pragma once
#include "movableobj.h"
#include "mapmngr.h"

class EnemyMngr;
class SkillOrb;



class Enemy : public MovableObj
{
private:
    EnemyMngr* enemy_mngr_;
    EnemyMngr* enemy_type_;
    bool dead = false; //����ł��邩�ǂ���
    int atk_ = 0;
public:
    Enemy() = delete;
    Enemy(int x, int y,int texNo,EnemyMngr* enemy_mngr) : MovableObj(Vector2((x + 0.5f) * SIZE_, (y + 0.5f) * SIZE_),0.0f,texNo,Vector2(0.0f,0.0f))
    {
        enemy_mngr_ = enemy_mngr;
    }
    virtual ~Enemy() = default;
    virtual void Update() = 0;
    EnemyMngr* GetEnemyMngr() const { return enemy_mngr_; }
    bool IsDead() const { return dead; }
    void Die(){ this->dead = true; }

    virtual SkillOrb* DropSkillOrb() = 0;

    EnemyMngr* GetEnemyType() const { return enemy_type_; }

    int GetAtk(void) const { return atk_; }
    void SetAtk(int atk) { atk_ = atk; }

};
