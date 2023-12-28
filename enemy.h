#pragma once
#include "movableobj.h"
#include "mapmngr.h"

class EnemyMngr;
class SkillOrb;


enum ENEMY_TYPE
{
    ENEMY_TYPE_KOOPA,
    ENEMY_TYPE_HAMMERBRO,
    ENEMY_TYPE_PHANTOM,
};


class Enemy : public MovableObj
{
private:
    EnemyMngr* enemy_mngr_;
    bool dead = false; //Ž€‚ñ‚Å‚¢‚é‚©‚Ç‚¤‚©
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

    static ENEMY_TYPE GetEnemyType(MAP_READ type);
};
