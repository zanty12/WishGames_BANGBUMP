#include "RectColl.h"

RectColl::RectColl(GameObject* parent) : Collider(RECTANGLE, parent)
{
    Vertex4 rect = Vertex4(Vector2(parent->GetPos().x - parent->GetScale().x / 2, parent->GetPos().y + parent->GetScale().y / 2),
                            Vector2(parent->GetPos().x + parent->GetScale().x / 2, parent->GetPos().y + parent->GetScale().y / 2),
                            Vector2(parent->GetPos().x + parent->GetScale().x / 2, parent->GetPos().y - parent->GetScale().y / 2),
                            Vector2(parent->GetPos().x - parent->GetScale().x / 2, parent->GetPos().y - parent->GetScale().y / 2));//TODO: get rect from parent
    rect_ = rect;
}
