#pragma once
#include "enemy.h"
#include <vector>
#include "mapenum.h"

class MapMngr;
class Enemy;

class EnemyMngr
{
private:
    std::vector<Enemy*> enemies_;
    MapMngr* map_mngr_;
public:
    EnemyMngr(MapMngr* map_mngr){
        map_mngr_ = map_mngr;
        enemies_.reserve(20);
    }

    ~EnemyMngr()
    {
        for(Enemy* enemy : enemies_)
        {
            delete enemy;
        }
        enemies_.clear();
    }
    void Update();
    void Spawn(int x,int y,int type);
    void PutEnemy(Enemy* enemy)
    {
        enemies_.push_back(enemy);
    }
    MapMngr* GetMapMngr() const { return map_mngr_; }
    std::vector<Enemy*> GetEnemies() const { return enemies_; }
};
