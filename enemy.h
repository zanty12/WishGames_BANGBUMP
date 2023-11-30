#pragma once
#include "movableobj.h"
#include "mapmngr.h"

class EnemyMngr;

class Enemy : public MovableObj
{
private:
    EnemyMngr* enemy_mngr_;
    bool dead = false; //€‚ñ‚Å‚¢‚é‚©‚Ç‚¤‚©
public:
    Enemy() = delete;
    Enemy(int x, int y,EnemyMngr* enemy_mngr) : MovableObj(Vector2((x + 0.5) * size_, (y + 0.5) * size_),0.0f,0,Vector2(0.0f,0.0f))
    {
        enemy_mngr_ = enemy_mngr;
    }
    virtual ~Enemy() = default;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    EnemyMngr* GetEnemyMngr() const { return enemy_mngr_; }
    bool IsDead() const { return dead; }
    void Die(){ this->dead = true; }
};
