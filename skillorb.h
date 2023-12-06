#pragma once
#include "cell.h"
#include "mapenum.h"
#include "texture.h"

typedef enum
{
    ORB_SMALL = 1,
    ORB_MID,
    ORB_BIG,
} ORB_TYPE;

class SkillOrb :
    public Cell
{
private:
    ORB_TYPE type_;
    float size_ = GameObject::SIZE_;

public:
    SkillOrb(int x, int y, MAP_READ type) : Cell(x, y, LoadTexture("data/TEXTURE/skillorb.png"))
    {
        switch (type)
        {
        case MAP_READ_ORB_SMALL:
            type_ = ORB_SMALL;
            break;
        case MAP_READ_ORB_MID:
            type_ = ORB_MID;
            this->SetPos(this->GetPos() + Vector2(GameObject::SIZE_ / 2, -GameObject::SIZE_ / 2)); //2*2ÇÃê^ÇÒíÜÇ…Ç∏ÇÍÇÈ
            size_ = GameObject::SIZE_ * type_;
            break;
        case MAP_READ_ORB_BIG:
            type_ = ORB_BIG;
            this->SetPos(this->GetPos() + Vector2(GameObject::SIZE_, -GameObject::SIZE_)); //3*3ÇÃê^ÇÒíÜÇ…Ç∏ÇÍÇÈ
            size_ = GameObject::SIZE_ * type_;
            break;
        default:
            break;
        }
    }

    virtual ~SkillOrb() override = default;

    MAP_READ GetCellType() const override
    {
        switch (type_)
        {
        case (ORB_SMALL):
            return MAP_READ_ORB_SMALL;
        case (ORB_MID):
            return MAP_READ_ORB_MID;
        case (ORB_BIG):
            return MAP_READ_ORB_BIG;
        default:
            return MAP_READ_ORB_SMALL;
        }
    }
};
