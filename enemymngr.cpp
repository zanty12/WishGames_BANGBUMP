#include "enemymngr.h"
#include "koopa.h"

void EnemyMngr::Update() const
{
    for (Enemy* enemy : enemies_)
    {
        enemy->Update();
    }
}

void EnemyMngr::Draw() const
{
    for (Enemy* enemy : enemies_)
    {
        enemy->Draw();
    }
}

void EnemyMngr::Spawn(int x, int y, int type)
{
    //TODO: Ç±Ç±Ç≈ìGÇê∂ê¨Ç∑ÇÈ
    switch (type)
    {
    case(MAP_READ_KOOPA):
        enemies_.push_back(new Koopa(x, y));
        break;
    case(MAP_READ_HAMMERBRO):
        break;
    case(MAP_READ_PHANTOM):
        break;
    default:
        break;
    }
}
