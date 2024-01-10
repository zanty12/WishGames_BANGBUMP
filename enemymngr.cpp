#include "enemymngr.h"
#include "Enemy1.h"
#include "Enemy2.h"
#include "Enemy3.h"
#include "boss.h"

void EnemyMngr::Update()
{
    //破棄する
    enemies_.remove_if(
        [](Enemy* enemy) {
            if (enemy->IsDead()) {
                delete enemy;
                enemy = nullptr;
                return true;
            }
            return false;
        }
    );

    //アップデート
    for (auto enemy : enemies_)
    {
        if (enemy == nullptr)
            continue;

        enemy->Update();

    }
}


void EnemyMngr::Spawn(int x, int y, int type)
{
    //TODO: ここで敵を生成する
    switch (type)
    {
    case(MAP_READ_KOOPA):
    {
        Enemy* enemy = new Enemy1(x, y, this);
        enemies_.push_back(enemy);
        break;
    }
        break;
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
    case(MAP_READ_ODIN):
    {
        Enemy* enemy = new Boss(x, y, this);
        enemies_.push_back(enemy);
        break;
    }
        break;
    default:
        break;
    }
}

