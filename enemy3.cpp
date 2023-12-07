#include "enemy1.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"

void Enemy1::Update()
{
    Player* player = GetEnemyMngr()->GetMapMngr()->GetGame()->GetPlayer(); //��������̃��������쑽����

    GetAnimator()->SetIsAnim(true);

    float dt = Time::GetDeltaTime() < 1 ? Time::GetDeltaTime() : 0.0f; //���������̃G���[���������

    //�d��
    SetVel(Vector2(GetVel().x, GetVel().y - y_spd_ * dt));

    float once = 3.0f;//���

    if (Time::GetDeltaTime() > once)
    {

    }



    //�v���C���[�Ǐ]
    if (player->GetPos().x < GetPos().x)
    {
        SetVel(Vector2(-x_spd_ * dt, GetVel().y));
    }
    else if (player->GetPos().x > GetPos().x)
    {
        SetVel(Vector2(x_spd_ * dt, GetVel().y));
    }
    else
    {
        SetVel(Vector2(0.0f, GetVel().y));
    }

    this->AddVel(GetVel());

    //�ǔ���
    CellActions();
    //�v���C���[�Ƃ̓����蔻��
    if (Collision(player))
    {
        player->HpDown(500);//������
        //Die();
    }
    //���̓G�Ƃ̓����蔻��
    for (auto& enemy : GetEnemyMngr()->GetEnemies())
    {
        if (enemy == this || enemy == nullptr)
            continue;
        if (Collision(enemy))
        {
            //enemy->SetVel(Vector2(enemy->GetVel().x, 0.5f));
        }
    }
}

void Enemy1::CellActions()
{
    Map* map = GetEnemyMngr()->GetMapMngr()->GetMap();
    Cell* cells[4] = { nullptr, nullptr, nullptr, nullptr };
    int idx = std::floor(GetPos().x / SIZE_);
    int idy = std::floor(GetPos().y / SIZE_);
    cells[0] = map->GetCell(idx, idy + 1);
    cells[1] = map->GetCell(idx, idy - 1);
    cells[2] = map->GetCell(idx - 1, idy);
    cells[3] = map->GetCell(idx + 1, idy);
    for (int i = 0; i < 4; i++)
    {
        if (cells[i] == nullptr)
            continue;
        if (Collision(cells[i]))
            MapCellInteract(cells[i]);
    }
}
