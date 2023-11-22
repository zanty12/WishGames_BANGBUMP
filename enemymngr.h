#pragma once
#include "enemy.h"
#include <vector>
#include "mapenum.h"

class EnemyMngr
{
private:
    std::vector<Enemy*> enemies_;
public:
    EnemyMngr() {
        enemies_.reserve(20);
    }
    ~EnemyMngr()
    {
        for(Enemy* enemy : enemies_)
        {
            delete enemy;
        }
    }
    void Update() const;
    void Draw() const;
    void Spawn(int x,int y,int type);
    void PutEnemy(Enemy* enemy)
    {
        enemies_.push_back(enemy);
    }
};
