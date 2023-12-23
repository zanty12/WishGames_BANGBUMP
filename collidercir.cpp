#include "collidercir.h"
#include "animator.h"
#include "gamebase.h"

ColliderCir::ColliderCir(GameObject* parent, float radius): Collider(CIRCLE, parent)
{
    cir_.a = parent->GetPos();
    radius_ = radius;

    GameBase::GetCollMngr()->Add(this);
}

bool ColliderCir::Collide(Collider* other)
{
    switch (other->GetType())
    {
    case CIRCLE:
        if (Collider2D::Touch(cir_, dynamic_cast<ColliderCir*>(other)->GetCircle()))
        {
            collision_.push_back(other);
            return true;
        }
        return false;
    case RECTANGLE:
        if(Collider2D::Touch(cir_, dynamic_cast<ColliderRect*>(other)->GetRect()))
        {
            collision_.push_back(other);
            return true;
        }
        return false;
    default:
        return false;
    }
}

void ColliderCir::Update()
{
    if (GetPos() != GetParent()->GetPos())
    {
        SetPos(GetParent()->GetPos());
        cir_.a = GetPos();
    }
}

void ColliderCir::CollisionInteract()
{
    for (auto& other : collision_)
    {
        switch (other->GetParent()->GetType())
        {
        case OBJ_SOLID:
            CollisionSolid(other);
            break;
        case OBJ_PENETRABLE:
            CollisionPen(other);
            break;
        case OBJ_VOID:
            break;
        default:
            CollisionSolid(other);
        }
    }
    GetParent()->SetPos(GetPos());
}

void ColliderCir::CollisionSolid(Collider* other)
{
    switch (other->GetType())
    {
    case CIRCLE:
        {
            //get vector from this to other
            Vector2 vec = other->GetPos() - GetPos();
            //normalize
            Vector2 dir = vec;
            dir.Normalize();
            //get move vector
            const Vector2 move = dir * (GetRadius() + dynamic_cast<ColliderCir*>(other)->GetRadius() - vec.Distance());
            //move this
            SetPos(GetPos() - move);
        }
        break;
    case RECTANGLE:
        {
            ColliderRect* otherRect = dynamic_cast<ColliderRect*>(other);
            Vertex4 rect = otherRect->GetRect();
            Vector2 circleCenter = GetPos();
            Vector2 closest = circleCenter; // Initialize to circle center

            // Check each side of the rectangle
            for (int i = 0; i < 4; i++)
            {
                Vector2 rectSideStart = rect.v[i];
                Vector2 rectSideEnd = rect.v[(i + 1) % 4];
                Vector2 sideDir = (rectSideEnd - rectSideStart).Normalize();

                // Project circle center onto rectangle side
                float t = Vector2::Dot((circleCenter - rectSideStart), sideDir);
                Vector2 projection = rectSideStart + sideDir * t;

                // Clamp projection to rectangle side
                float sideLength = (rectSideEnd - rectSideStart).Distance();
                if (t < 0) projection = rectSideStart;
                else if (t > sideLength) projection = rectSideEnd;

                // If this projection is closer to circle center, update closest
                if ((projection - circleCenter).Distance() < (closest - circleCenter).Distance())
                {
                    closest = projection;
                }
            }

            Vector2 collisionNormal = circleCenter - closest;
            float overlap = GetRadius() - collisionNormal.Distance();
            if (overlap > 0)
            {
                collisionNormal.Normalize();
                SetPos(GetPos() + collisionNormal * overlap);
            }
        }
        break;
    }
}

void ColliderCir::CollisionPen(Collider* other)
{
    switch (other->GetType())
    {
    case CIRCLE:
        {
            //get vector from this to other
            Vector2 vec = other->GetPos() - GetPos();
            //normalize
            Vector2 dir = vec;
            dir.Normalize();
            //get move vector
            const Vector2 move = dir * (GetRadius() + dynamic_cast<ColliderCir*>(other)->GetRadius() - vec.Distance());
            //move this
            SetPos(GetPos() - move);
        }
        break;
    case RECTANGLE:
        {
            //handle collision but make the object go through only from the bottom of other
            ColliderRect* other_rect = dynamic_cast<ColliderRect*>(other);
            Vertex4 rect = other_rect->GetRect();
            Vector2 circle_center = GetPos();
            Vector2 closest = circle_center; // Initialize to circle center
            // Check each side of the rectangle
            for (int i = 0; i < 4; i++)
            {
                Vector2 rect_side_start = rect.v[i];
                Vector2 rect_side_end = rect.v[(i + 1) % 4];
                Vector2 side_dir = (rect_side_end - rect_side_start).Normalize();

                // Project circle center onto rectangle side
                float t = Vector2::Dot((circle_center - rect_side_start), side_dir);
                Vector2 projection = rect_side_start + side_dir * t;

                // Clamp projection to rectangle side
                float side_length = (rect_side_end - rect_side_start).Distance();
                if (t < 0) projection = rect_side_start;
                else if (t > side_length) projection = rect_side_end;

                // If this projection is closer to circle center, update closest
                if ((projection - circle_center).Distance() < (closest - circle_center).Distance())
                {
                    closest = projection;
                }
            }

            //go through if the collision is from the bottom
            if (closest.y == rect.v[0].y || closest.y == rect.v[1].y)
            {
                Vector2 collision_normal = circle_center - closest;
                float overlap = GetRadius() - collision_normal.Distance();
                if (overlap > 0)
                {
                    collision_normal.Normalize();
                    SetPos(GetPos() + collision_normal * overlap);
                }
            }
            else if (closest.y == rect.v[2].y || closest.y == rect.v[3].y)
            {
                //go straight through to the top
                SetPos(Vector2(GetPos().x, GetPos().y + GetRadius() + other->GetParent()->GetScale().y / 2));
            }
        }
        break;
    }
}
