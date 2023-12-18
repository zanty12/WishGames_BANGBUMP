#pragma once
#pragma once
#include "skillorb.h"
#include "attribute.h"


typedef enum
{
    ORB_FIRE = 1,
    ORB_DARK,
    ORB_THUNDER,
    ORB_WIND,
} ORB_ATTRIBUTE;

class SkillOrbAttribute :public SkillOrb
{
private:

    ORB_ATTRIBUTE attribute_;
    float size_ = GameObject::SIZE_;

public:
    SkillOrbAttribute(int x, int y, MAP_READ type)
        :SkillOrb(x, y, type)
    {
        bool set_attribute = false;
        if (type >= MAP_READ_ORB_SMALL_FIRE && type <= MAP_READ_ORB_BIG_THUNDER)
        {
            int orb = type - MAP_READ_ORB_SMALL_FIRE;
            int i = 0;
            for (int a = 0; a < 4; a++)
            {
                for (int s = 0; s < ORB_BIG; s++)
                {
                    if (orb == i) {
                        attribute_ = (ORB_ATTRIBUTE)a;
                        set_attribute = true;
                        break;
                    }
                    i++;
                }
                if (set_attribute)
                    break;
            }

        }


    }
    ~SkillOrbAttribute() {}

    MAP_READ GetCellType() const override
    {
        switch (GetType())
        {
        case ORB_SMALL:
            switch (attribute_)
            {
            case (ORB_FIRE):
                return MAP_READ_ORB_SMALL_FIRE;
            case (ORB_DARK):
                return MAP_READ_ORB_SMALL_DARK;
            case (ORB_WIND):
                return MAP_READ_ORB_SMALL_WIND;
            case (ORB_THUNDER):
                return MAP_READ_ORB_SMALL_THUNDER;
            }
            break;
        case ORB_MID:
            switch (attribute_)
            {
            case (ORB_FIRE):
                return MAP_READ_ORB_MID_FIRE;
            case (ORB_DARK):
                return MAP_READ_ORB_MID_DARK;
            case (ORB_WIND):
                return MAP_READ_ORB_MID_WIND;
            case (ORB_THUNDER):
                return MAP_READ_ORB_MID_THUNDER;
            }
            break;
        case ORB_BIG:
            switch (attribute_)
            {
            case (ORB_FIRE):
                return MAP_READ_ORB_BIG_FIRE;
            case (ORB_DARK):
                return MAP_READ_ORB_BIG_DARK;
            case (ORB_WIND):
                return MAP_READ_ORB_BIG_WIND;
            case (ORB_THUNDER):
                return MAP_READ_ORB_BIG_THUNDER;
            }
            break;
        default:
            break;
        }

    }
private:

};