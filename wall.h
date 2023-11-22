#pragma once
#include "cell.h"
#include "texture.h"

class Wall :
    public Cell
{
public:
    Wall(int x, int y) : Cell(x,y,0){
        this->SetTexNo(LoadTexture("data/TEXTURE/wall.png"));
    }
    virtual ~Wall() override = default;
    MAP_READ GetCellType() const override { return MAP_READ_WALL; }
};

