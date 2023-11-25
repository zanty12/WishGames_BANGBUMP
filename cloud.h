#pragma once
#include "cell.h"
#include "texture.h"
class Cloud :
    public Cell
{
public:
    Cloud(int x, int y) : Cell(x, y, 0) {
        this->SetTexNo(LoadTexture("data/texture/cloud.png"));
    }
    virtual ~Cloud() override = default;
    MAP_READ GetCellType() const override { return MAP_READ_CLOUD; }
};

