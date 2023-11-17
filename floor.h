#pragma once
#include "cell.h"
class Floor :
    public Cell
{
public:
    Floor(int x, int y) : Cell(x, y, 0) {} //TODO : change 0 to Floor type
    virtual ~Floor() override = default;
};

