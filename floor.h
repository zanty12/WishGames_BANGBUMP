#pragma once
#include "cell.h"
#include "texture.h"

class Floor :
    public Cell
{
public:
    Floor(int x, int y) : Cell(x, y, LoadTexture("data/texture/floor.png")){}
    ~Floor() override = default;
    MAP_READ GetCellType() const override { return MAP_READ_FLOOR; }
};

