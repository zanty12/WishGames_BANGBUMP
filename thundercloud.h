#pragma once
#include "cell.h"
class ThunderCloud :
    public Cell
{
public:
    ThunderCloud(int x, int y) : Cell(x, y, 0) {} //TODO : change 0 to Cloud type
    virtual ~ThunderCloud() override = default;
};

