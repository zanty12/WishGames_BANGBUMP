#pragma once
#include "cell.h"
#include "texture.h"
class PenFloor :
    public Cell
{
public:
    PenFloor(int x, int y) : Cell(x, y, 0) {} //TODO : change 0 to PenFloor type
    virtual ~PenFloor() override = default;
};

