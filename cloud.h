#pragma once
#include "cell.h"
#include "texture.h"
class Cloud :
    public Cell
{
public:
    Cloud(int x, int y) : Cell(x, y, 0) {
        this->SetTexNo(LoadTexture("data/TEXTURE/cloud.png"));
    }
    virtual ~Cloud() override = default;
};

