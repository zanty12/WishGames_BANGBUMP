#include "enemymngr.h"
#include "Enemy1.h"
#include "Enemy2.h"
#include "Enemy3.h"

void EnemyMngr::Update()
{
    /*for(int i = 0; i < enemies_.size(); i++)
    {
        if(enemies_[i] == nullptr)
            continue;

        enemies_[i]->Update();

        if(enemies_[i]->IsDead())
        {
            delete enemies_[i];
            enemies_[i] = nullptr;
        }
    }*/
    for (auto enemy : enemies_)
    {
        if (enemy == nullptr)
            continue;

        enemy->Update();

        if (enemy->IsDead())
        {
            delete enemy;
            enemy = nullptr;
        }
    }
}


void EnemyMngr::Spawn(int x, int y, int type)
{
    //TODO: Ç±Ç±Ç≈ìGÇê∂ê¨Ç∑ÇÈ
    switch (type)
    {
    case(MAP_READ_KOOPA):
        {
            Enemy* enemy = new Enemy1(x, y,this);
            enemies_.push_back(enemy);
            break;
        }

    case(MAP_READ_HAMMERBRO):
    {
        Enemy* enemy = new Enemy2(x, y, this);
        enemies_.push_back(enemy);
        break;
    }
        break;
    case(MAP_READ_PHANTOM):
    {
        Enemy* enemy = new Enemy3(x, y, this);
        enemies_.push_back(enemy);
        break;
    }
        break;
    default:
        break;
    }
}
