#pragma once
#include "movableobj.h"
#include "mapmngr.h"
#include "skillorb.h"

class EnemyMngr;



enum ENEMY_TYPE
{
    TYPE__KOOPA,
    TYPE__HAMMERBRO,
    TYPE__PHANTOM,
};


class Enemy : public MovableObj
{
private:
    EnemyMngr* enemy_mngr_;
    ENEMY_TYPE enemy_type_;
    bool dead = false; //死んでいるかどうか
    int atk_;
    int hp_;
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

    //virtual SkillOrb* DropSkillOrb() = 0;

    void DropSkillOrb(Vector2 pos, SKILLORB_SIZE_TYPE sizeType);

    ENEMY_TYPE GetEnemyType() const { return enemy_type_; }

    int GetAtk(void) const { return atk_; }
    void SetAtk(int atk) { atk_ = atk; }
    int GetHp(void) const { return hp_; }
    void SetHp(int hp) { hp_ = hp; }
    //HPの減少（ダメージが現在のHPを超える場合、HPは0になる）
    void HpDown(int damage) { damage <= hp_ ? hp_ -= damage : hp_ = 0; }

};
