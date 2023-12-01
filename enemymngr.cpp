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

void EnemyMngr::Draw(Camera* camera) const
{
    for (Enemy* enemy : enemies_)
    {
        if(enemy != nullptr && camera->InCamera(enemy))
            enemy->Draw(camera);
    }
}

void EnemyMngr::Spawn(int x, int y, int type)
{
    //TODO: Ç±Ç±Ç≈ìGÇê∂ê¨Ç∑ÇÈ
    switch (type)
    {
    case(MAP_READ_KOOPA):
        enemies_.push_back(new Koopa(x, y,this));
        break;
    case(MAP_READ_HAMMERBRO):
        break;
    case(MAP_READ_PHANTOM):
        break;
    default:
        break;
    }
}
