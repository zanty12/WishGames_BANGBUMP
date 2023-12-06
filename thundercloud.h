#pragma once
#include "cell.h"
#include "texture.h"

class ThunderCloud :
    public Cell
{
public:
    ThunderCloud(int x, int y) : Cell(x, y, LoadTexture("data/TEXTURE/thundercloud.png")) {}
    virtual ~ThunderCloud() override = default;
    MAP_READ GetCellType() const override { return MAP_READ_THUNDERCLOUD; }
};

