#pragma once
#include "cell.h"
#include "texture.h"
class PenFloor :
    public Cell
{
public:
    PenFloor(int x, int y) : Cell(x, y, LoadTexture("data/texture/penfloor.png")) {}
    virtual ~PenFloor() override = default;
    MAP_READ GetCellType() const override { return MAP_READ_PENFLOOR; }
};

