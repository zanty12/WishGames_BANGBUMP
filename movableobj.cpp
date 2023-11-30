#include "movableobj.h"
#include "lib/collider2d.h"

bool MovableObj::Collision(const GameObject* obj) const
{
    using namespace PHYSICS;
    Vertex4 Self = Vertex4(Vector2(GetPos().x - GetScale().x / 2, GetPos().y + GetScale().y / 2),
                           Vector2(GetPos().x + GetScale().x / 2, GetPos().y + GetScale().y / 2),
                           Vector2(GetPos().x + GetScale().x / 2, GetPos().y - GetScale().y / 2),
                           Vector2(GetPos().x - GetScale().x / 2, GetPos().y - GetScale().y / 2));
    Vertex4 other = Vertex4(Vector2(obj->GetPos().x - GetScale().x / 2, obj->GetPos().y + GetScale().y / 2),
                            Vector2(obj->GetPos().x + GetScale().x / 2, obj->GetPos().y + GetScale().y / 2),
                            Vector2(obj->GetPos().x + GetScale().x / 2, obj->GetPos().y - GetScale().y / 2),
                            Vector2(obj->GetPos().x - GetScale().x / 2, obj->GetPos().y - GetScale().y / 2));

    return Collider2D::Touch(Self, other);
}

void MovableObj::MapCellInteract(Cell* cell)
{
    switch (cell->GetCellType())
    {
    case MAP_READ_WALL:
        SolidCellInteract(cell);
        break;
    case MAP_READ_FLOOR:
        SolidCellInteract(cell);
        break;
    case MAP_READ_BLOCK:
        SolidCellInteract(cell);
        break;
    case MAP_READ_PENFLOOR:
        PenCellInteract(cell);
        break;
    case MAP_READ_CLOUD:
        PenCellInteract(cell);
    default:
        break;
    }
}

void MovableObj::SolidCellInteract(Cell* cell)
{
    const int self_idx = std::floor(GetPos().x / size_);
    const int self_idy = std::floor(GetPos().y / size_);
    const int cell_idx = std::floor(cell->GetPos().x / size_);
    const int cell_idy = std::floor(cell->GetPos().y / size_);
    const int x = cell_idx - self_idx;
    const int y = cell_idy - self_idy;

    if (x == 0 && y == 1) //マスの下
    {
        SetPos(Vector2(GetPos().x, cell->GetPos().y - size_));
        SetVel(Vector2(GetVel().x, 0.0f));
    }
    else if (x == 0 && y == -1) //マスの上
    {
        SetPos(Vector2(GetPos().x, cell->GetPos().y + size_));
        SetVel(Vector2(GetVel().x, 0.0f));
    }
    else if (x == 1 && y == 0) //マスの右
    {
        SetPos(Vector2(cell->GetPos().x - size_, GetPos().y));
    }
    else if (x == -1 && y == 0) //マスの左
    {
        SetPos(Vector2(cell->GetPos().x + size_, GetPos().y));
    }
}

void MovableObj::PenCellInteract(Cell* cell)
{
    const int self_idx = std::floor(GetPos().x / size_);
    const int self_idy = std::floor(GetPos().y / size_);
    const int cell_idx = std::floor(cell->GetPos().x / size_);
    const int cell_idy = std::floor(cell->GetPos().y / size_);
    const int x = cell_idx - self_idx;
    const int y = cell_idy - self_idy;
    if (x == 0 && y == 1) //マスの下
    {
        //貫通するから何もしない
    }
    else if (x == 0 && y == -1) //マスの上
    {
        SetPos(Vector2(GetPos().x, cell->GetPos().y + size_));
        SetVel(Vector2(GetVel().x, 0.0f));
    }
    else if (x == 1 && y == 0) //マスの右
    {
        SetPos(Vector2(cell->GetPos().x - size_, GetPos().y));
    }
    else if (x == -1 && y == 0) //マスの左
    {
        SetPos(Vector2(cell->GetPos().x + size_, GetPos().y));
    }
}
