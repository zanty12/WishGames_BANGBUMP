#include "koopa.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"

void Koopa::Update()
{
    Player* player = GetEnemyMngr()->GetMapMngr()->GetGame()->GetPlayer(); //正直これのメモリ操作多すぎ
    //Get to the player only horizontally
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
    this->AddVel(GetVel());
}


void Koopa::GetNearCells() const
{
    Map* map = GetEnemyMngr()->GetMapMngr()->GetMap();
    Cell* up = map->GetCell(GetPos().x, GetPos().y+size_);
    Cell* down = map->GetCell(GetPos().x, GetPos().y-size_);
    Cell* left = map->GetCell(GetPos().x-size_, GetPos().y);
    Cell* right = map->GetCell(GetPos().x+size_, GetPos().y);

}

bool Koopa::CellCollision(Cell* cell) const
{
    using namespace PHYSICS;
    Vertex4 Self = Vertex4(Vector2(GetPos().x - size_ / 2,GetPos().y + size_ / 2),
                            Vector2(GetPos().x + size_ / 2,GetPos().y + size_ / 2),
                            Vector2(GetPos().x + size_ / 2,GetPos().y - size_ / 2),
                            Vector2(GetPos().x - size_ / 2,GetPos().y - size_ / 2));
    Vertex4 other  = Vertex4(Vector2(cell->GetPos().x - size_ / 2,cell->GetPos().y + size_ / 2),
                            Vector2(cell->GetPos().x + size_ / 2,cell->GetPos().y + size_ / 2),
                            Vector2(cell->GetPos().x + size_ / 2,cell->GetPos().y - size_ / 2),
                            Vector2(cell->GetPos().x - size_ / 2,cell->GetPos().y - size_ / 2));

    return false;
}
