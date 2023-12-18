#pragma once
#include "cell.h"
#include "mapenum.h"
#include "texture.h"

typedef enum
{
    ORB_EMPTY = 0,
    ORB_SMALL = 1,
    ORB_MID,
    ORB_BIG,
} ORB_TYPE;

class SkillOrb:
    public Cell
{
private:
    ORB_TYPE type_ = ORB_EMPTY;
    float size_ = GameObject::SIZE_;

public:
    SkillOrb() = delete;
    SkillOrb(int x, int y, MAP_READ type) : Cell(x, y, LoadTexture("data/TEXTURE/skillorb.png"))
        ,type_(ORB_EMPTY)
    {
        bool set_type = false;
        if (type >= MAP_READ_ORB_SMALL_FIRE && type <= MAP_READ_ORB_BIG_THUNDER)
        {
            int orb = type - MAP_READ_ORB_SMALL_FIRE;
            int i = 0;
            for (int a = 0; a < 4; a++)
            {
                for (int s = 1; s <= ORB_BIG; s++)
                {
                    if (orb == i) {
                        type_ = (ORB_TYPE)s;
                        set_type = true;
                        break;
                    }
                    i++;
                }
                if (set_type)
                    break;
            }

        }

        switch (type_)
        {
        case ORB_SMALL:
            break;
        case ORB_MID:
            this->SetPos(this->GetPos() + Vector2(GameObject::SIZE_ / 2, -GameObject::SIZE_ / 2)); //2*2の真ん中にずれる
            size_ = GameObject::SIZE_ * type_;
            break;
        case ORB_BIG:
            this->SetPos(this->GetPos() + Vector2(GameObject::SIZE_, -GameObject::SIZE_)); //3*3の真ん中にずれる
            size_ = GameObject::SIZE_ * type_;
            break;
        default:
            break;
        }

    }

    ORB_TYPE GetType() const { return type_; }
    virtual ~SkillOrb() override = default;

    virtual MAP_READ GetCellType() const = 0;
};


//--------
// 
//           type_ = ORB_SMALL;
// 
//              type_ = ORB_MID;
//             this->SetPos(this->GetPos() + Vector2(GameObject::SIZE_ / 2, -GameObject::SIZE_ / 2)); //2*2の真ん中にずれる
//              size_ = GameObject::SIZE_ * type_;
// 
//              type_ = ORB_BIG;
//              this->SetPos(this->GetPos() + Vector2(GameObject::SIZE_, -GameObject::SIZE_)); //3*3の真ん中にずれる
//              size_ = GameObject::SIZE_ * type_;
//--------