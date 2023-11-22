#include "enemymngr.h"

void EnemyMngr::Update() const
{
    for(Enemy* enemy : enemies_)
    {
        enemy->Update();
    }
}

void EnemyMngr::Draw () const
{
    for (Enemy* enemy : enemies_)
    {
        enemy->Draw();
    }
}

void EnemyMngr::Spawn(int x, int y, int type)
{
    Enemy* enemy = nullptr; //TODO: �����œG�𐶐�����
    enemies_.push_back(enemy);
}

