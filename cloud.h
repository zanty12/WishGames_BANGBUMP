#pragma once
#include "cell.h"
class Cloud :
    public Cell
{
public:
    Cloud(int x, int y) : Cell(x, y, 0) {} //TODO : change 0 to Cloud type
    virtual ~Cloud() override = default;
};

