#pragma once
#include "cell.h"
class SkillOrb :
    public Cell
{
public:
    SkillOrb(int x, int y) : Cell(x, y, 0) {} //TODO : change 0 to SkillOrb type
    virtual ~SkillOrb() override = default;
};

