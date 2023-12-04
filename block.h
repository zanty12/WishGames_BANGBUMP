#pragma once
#include "cell.h"
class Block :
    public Cell
{
public:
    Block(int x, int y) : Cell(x, y, 0)
    {
        SetTexNo(LoadTexture("data/texture/block.png"));
    }
    virtual ~Block() override = default;
    MAP_READ GetCellType() const override { return MAP_READ_BLOCK; }
};

