#include "movableobj.h"
#include "lib/collider2d.h"

bool MovableObj::Collision(const GameObject* obj) const
{
    using namespace PHYSICS;
    Vertex4 Self = Vertex4(Vector2(GetPos().x - size_ / 2, GetPos().y + size_ / 2),
                           Vector2(GetPos().x + size_ / 2, GetPos().y + size_ / 2),
                           Vector2(GetPos().x + size_ / 2, GetPos().y - size_ / 2),
                           Vector2(GetPos().x - size_ / 2, GetPos().y - size_ / 2));
    Vertex4 other = Vertex4(Vector2(obj->GetPos().x - size_ / 2, obj->GetPos().y + size_ / 2),
                            Vector2(obj->GetPos().x + size_ / 2, obj->GetPos().y + size_ / 2),
                            Vector2(obj->GetPos().x + size_ / 2, obj->GetPos().y - size_ / 2),
                            Vector2(obj->GetPos().x - size_ / 2, obj->GetPos().y - size_ / 2));

    return Collider2D::Touch(Self, other);
}
