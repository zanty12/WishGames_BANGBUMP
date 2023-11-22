#pragma once
#include "cell.h"
#include "texture.h"

class Floor :
    public Cell
{
public:
    Floor(int x, int y) : Cell(x, y, 0)
    {
        this->SetTexNo(LoadTexture("data/TEXTURE/floor.png"));
    }
    virtual ~Floor() override = default;
    MAP_READ GetCellType() const override { return MAP_READ_FLOOR; }
};

