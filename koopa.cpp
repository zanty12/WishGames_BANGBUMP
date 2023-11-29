#include "koopa.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"

void Koopa::Update()
{
    Player* player = GetEnemyMngr()->GetMapMngr()->GetGame()->GetPlayer(); //正直これのメモリ操作多すぎ
    //重力
    SetVel(Vector2(GetVel().x, GetVel().y - 0.01f));
    //プレイヤー追従
    if (player->GetPos().x < GetPos().x)
    {
        SetVel(Vector2(-0.5f, GetVel().y));
    }
    else if (player->GetPos().x > GetPos().x)
    {
        SetVel(Vector2(0.5f, GetVel().y));
    }
    else
    {
        SetVel(Vector2(0.0f, GetVel().y));
    }
    //壁判定
    CellActions();

    this->AddVel(GetVel());
}

void Koopa::CellActions()
{
    Map* map = GetEnemyMngr()->GetMapMngr()->GetMap();
    Cell* cells[4] = {nullptr, nullptr, nullptr, nullptr};
    int idx = std::floor(GetPos().x / size_);
    int idy = std::floor(GetPos().y  / size_);
    cells[0] = map->GetCell(idx, idy + 1);
    cells[1] = map->GetCell(idx, idy - 1);
    cells[2] = map->GetCell(idx - 1, idy);
    cells[3] = map->GetCell(idx + 1, idy);
    for (int i = 0; i < 4; i++)
    {
        if (cells[i] != nullptr)
        {
            if (CellCollision(cells[i]) && i < 2)
            {
                SetVel(Vector2(GetVel().x, 0.0f));
                SetPos(Vector2(GetPos().x, cells[i]->GetPos().y + size_));
            }
            else if (CellCollision(cells[i]) && i >= 2)
            {
                SetVel(Vector2(0.0f, GetVel().y));
                /*if (i == 2)
                    SetPos(Vector2(GetPos().x + size_, GetPos().y));
                else
                {
                    SetPos(Vector2(GetPos().x - size_, GetPos().y));
                }*/
            }
        }
    }
}

bool Koopa::CellCollision(Cell* cell) const
{
    using namespace PHYSICS;
    Vertex4 Self = Vertex4(Vector2(GetPos().x - size_ / 2, GetPos().y + size_ / 2),
                           Vector2(GetPos().x + size_ / 2, GetPos().y + size_ / 2),
                           Vector2(GetPos().x + size_ / 2, GetPos().y - size_ / 2),
                           Vector2(GetPos().x - size_ / 2, GetPos().y - size_ / 2));
    Vertex4 other = Vertex4(Vector2(cell->GetPos().x - size_ / 2, cell->GetPos().y + size_ / 2),
                            Vector2(cell->GetPos().x + size_ / 2, cell->GetPos().y + size_ / 2),
                            Vector2(cell->GetPos().x + size_ / 2, cell->GetPos().y - size_ / 2),
                            Vector2(cell->GetPos().x - size_ / 2, cell->GetPos().y - size_ / 2));

    return Collider2D::Touch(Self, other);
}
