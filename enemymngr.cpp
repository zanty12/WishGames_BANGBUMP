#include "enemymngr.h"
#include "koopa.h"

void EnemyMngr::Update()
{
    for(int i = 0; i < enemies_.size(); i++)
    {
        if(enemies_[i] == nullptr)
            continue;

        enemies_[i]->Update();

        if(enemies_[i]->IsDead())
        {
            delete enemies_[i];
            enemies_[i] = nullptr;
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
            Enemy* enemy = new Koopa(x, y,this);
            enemies_.push_back(enemy);
            break;
        }

    case(MAP_READ_HAMMERBRO):
        break;
    case(MAP_READ_PHANTOM):
        break;
    default:
        break;
    }
}
