#pragma once
#include "cell.h"
class Block :
    public Cell
{
public:
    Block(int x, int y) : Cell(x, y, LoadTexture("data/texture/block.png")){}
    ~Block() override = default;
    MAP_READ GetCellType() const override { return MAP_READ_BLOCK; }
};

