#pragma once
#include "cell.h"
#include "mapenum.h"

typedef enum
{
    SPIKE_LEFT,
    SPIKE_RIGHT,
    SPIKE_UP,
    SPIKE_DOWN,
} SPIKE_DIR;

class Spike : public Cell
{
private:
    SPIKE_DIR dir_;
public:
    Spike(int x, int y, MAP_READ type) : Cell(x, y, 0)
    {
        this->SetTexNo(LoadTexture("data/texture/spike.png"));
        switch (type)
        {
        case MAP_READ_SPIKE_UP:
            SetRot(0.0f);
            dir_ = SPIKE_UP;
            break;
        case MAP_READ_SPIKE_RIGHT:
            SetRot(MATH::PI / 2.0f);
            dir_ = SPIKE_RIGHT;
            break;
        case MAP_READ_SPIKE_DOWN:
            SetRot(MATH::PI);
            dir_ = SPIKE_DOWN;
            break;
        case MAP_READ_SPIKE_LEFT:
            SetRot(MATH::PI * 3.0f / 2.0f);
            dir_ = SPIKE_LEFT;
            break;
        default:
            break;
        }
    }

    ~Spike() override = default;

    MAP_READ GetCellType() const override
    {
        switch (dir_)
        {
        case SPIKE_UP:
            return MAP_READ_SPIKE_UP;
        case SPIKE_RIGHT:
            return MAP_READ_SPIKE_RIGHT;
        case SPIKE_DOWN:
            return MAP_READ_SPIKE_DOWN;
        case SPIKE_LEFT:
            return MAP_READ_SPIKE_LEFT;
        default:
            return MAP_READ_NONE;
        }
    }
};
