#pragma once
#include "cell.h"
class Block :
    public Cell
{
public:
    Block(int x, int y) : Cell(x, y, 0) {} //TODO : change 0 to PenFloor type
    virtual ~Block() override = default;
    MAP_READ GetCellType() const override { return MAP_READ_BLOCK; }
};

