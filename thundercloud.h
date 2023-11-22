#pragma once
#include "cell.h"
#include "texture.h"

class ThunderCloud :
    public Cell
{
public:
    ThunderCloud(int x, int y) : Cell(x, y, 0) {
        this->SetTexNo(LoadTexture("data/TEXTURE/thundercloud.png"));
    }
    virtual ~ThunderCloud() override = default;
};

