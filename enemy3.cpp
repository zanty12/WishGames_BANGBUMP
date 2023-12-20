#include "enemy3.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"

#define RANGE (SIZE_ * 5.0f)                            //�͈�


bool CheckEnemy3Length(Vector2 a, Vector2 b, float len);

void Enemy3::Update()
{
    Player* player = GetEnemyMngr()->GetMapMngr()->GetGame()->GetPlayer(); //��������̃��������쑽����

    GetAnimator()->SetIsAnim(true);

    float dt = Time::GetDeltaTime(); //���������̃G���[���������

    //�d��
    //SetVel(Vector2(GetVel().x, GetVel().y - y_spd_ * dt));

    Range(startPosition.x, startPosition.y, GetPos().x, GetPos().y);


    //�v���C���[�Ǐ]
    if (CheckEnemy3Length(GetPos(), startPosition, RANGE))//��
    {
        if (cheakRange == true)
        {
            Vector2 v = startPosition - GetPos();
            SetVel(v.Normalize() * spd_ * dt);
        }
    }
    else
    {
        if (cheakRange == false)
        {

            Vector2 v = player->GetPos() - GetPos();
            SetVel(v.Normalize() * spd_ * dt);
        }
    }

    /*
    //�ǔ���
    CellActions();

    //�v���C���[�Ƃ̓����蔻��
    if (Collision(player))
    {
        player->HpDown(15);//������
        //Die();
    }
    */

    //���̓G�Ƃ̓����蔻��
   /* for (auto& enemy : GetEnemyMngr()->GetEnemies())
    {
        if (enemy == this || enemy == nullptr)
            continue;
        if (Collision(enemy))
        {
            enemy->SetVel(Vector2(enemy->GetVel().x, enemy->GetVel().y ));
        }
    }*/

    this->AddVel(GetVel());
}

/*
void Enemy3::CellActions()
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
        //if (Collision(cells[i]))
        //    MapCellInteract(cells[i]);
    }
}
*/

bool CheckEnemy3Length(Vector2 a, Vector2 b, float len)
{

    if (Vector2::Distance(a, b) > len)
    {
        return true;
    }

    return false;
}

void Enemy3::Range(float a, float b, float c, float d)
{
    // �~�̏��
    float x = a;
    float y = b;
    float radius_0 = RANGE;
    float radius_1 = 0.5f;

    float h = c - x;
    float i = d - y;
    float g = sqrt(h * h + i * i);

    if (g > radius_0)
    {
        cheakRange = true;
    }
    else if (g < radius_1)
    {
        cheakRange = false;
    }
}

