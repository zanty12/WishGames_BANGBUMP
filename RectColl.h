#pragma once
#include "collider.h"

using namespace PHYSICS;
class RectColl : public Collider
{
private:
    Vertex4 rect_;

public:
    RectColl() = delete;
    RectColl(GameObject* parent);
};
