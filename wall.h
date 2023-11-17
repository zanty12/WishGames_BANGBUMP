#pragma once
#include "cell.h"
class Wall :
    public Cell
{
public:
    Wall(int x, int y) : Cell(x,y,0){} //TODO : change 0 to wall type
    virtual ~Wall() override = default;
};

