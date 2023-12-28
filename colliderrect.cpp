#include "colliderrect.h"

#include "gamebase.h"

ColliderRect::ColliderRect(GameObject* parent) : Collider(RECTANGLE, parent)
{
    rect_ = Vertex4(
        Vector2(parent->GetPos().x - parent->GetScale().x / 2, parent->GetPos().y + parent->GetScale().y / 2),
        Vector2(parent->GetPos().x + parent->GetScale().x / 2, parent->GetPos().y + parent->GetScale().y / 2),
        Vector2(parent->GetPos().x + parent->GetScale().x / 2, parent->GetPos().y - parent->GetScale().y / 2),
        Vector2(parent->GetPos().x - parent->GetScale().x / 2, parent->GetPos().y - parent->GetScale().y / 2));
    GameBase::GetCollMngr()->Add(this);
}

bool ColliderRect::Collide(Collider* other)
{
    switch (other->GetType())
    {
    case CIRCLE:
        return Collider2D::Touch(rect_, dynamic_cast<ColliderCir*>(other)->GetCircle());
    case RECTANGLE:
        //return Collider2D::Touch(rect_, dynamic_cast<ColliderRect*>(other)->GetRect());
        {
            //bounding box
            Vertex4 other_rect = dynamic_cast<ColliderRect*>(other)->GetRect();
            return (rect_.a.x < other_rect.b.x && rect_.b.x > other_rect.a.x &&
                rect_.a.y > other_rect.c.y && rect_.c.y < other_rect.a.y);
        }
    default:
        return false;
    }
}

void ColliderRect::Update()
{
    if (GetPos() != GetParent()->GetPos())
    {
        SetPos(GetParent()->GetPos());
        rect_.a = Vector2(GetPos().x - GetParent()->GetScale().x / 2, GetPos().y + GetParent()->GetScale().y / 2);
        rect_.b = Vector2(GetPos().x + GetParent()->GetScale().x / 2, GetPos().y + GetParent()->GetScale().y / 2);
        rect_.c = Vector2(GetPos().x + GetParent()->GetScale().x / 2, GetPos().y - GetParent()->GetScale().y / 2);
        rect_.d = Vector2(GetPos().x - GetParent()->GetScale().x / 2, GetPos().y - GetParent()->GetScale().y / 2);
    }
}

void ColliderRect::CollisionInteract()
{
    for (auto& other : collision_)
    {
        switch (other->GetParent()->GetType())
        {
        case OBJ_SOLID:
            CollisionSolid(other);
            break;
        case OBJ_PENETRABLE:
            {
                if (GetBounciness() > 1.0f)
                    CollisionSolid(other);
                else
                    CollisionPen(other);
            }
            break;
        case OBJ_VOID:
            break;
        case OBJ_ITEM:
            break;
        default:
            CollisionSolid(other);
        }
    }
    GetParent()->SetPos(GetPos());
}

void ColliderRect::CollisionSolid(Collider* other)
{
    switch (other->GetType())
    {
    case CIRCLE:
        {
            ColliderCir* other_cir = dynamic_cast<ColliderCir*>(other);
            Vertex4 rect = GetRect();
            Vector2 circle_center = other_cir->GetPos();
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

            Vector2 collision_normal = circle_center - closest;
            float overlap = other_cir->GetRadius() - collision_normal.Distance();
            if (overlap > 0)
            {
                collision_normal.Normalize();
                SetPos(GetPos() - collision_normal * overlap);
            }
        }
        break;
    case RECTANGLE:
        {
            ColliderRect* other_rect = dynamic_cast<ColliderRect*>(other);
            Vertex4 rect = GetRect();
            Vertex4 other_rect_ = other_rect->GetRect();
            Vector2 collision_normal = other->GetPos() - GetPos();
            float overlap_x = abs(collision_normal.x) - GetParent()->GetScale().x / 2 - other->GetParent()->GetScale().x
                / 2;
            float overlap_y = abs(collision_normal.y) - GetParent()->GetScale().y / 2 - other->GetParent()->GetScale().y
                / 2;
            if (overlap_x < 0 || overlap_y < 0)
            {
                Vector2 coll_dir = collision_normal.Normalize();
                Vector2 overlap = Vector2(overlap_x, overlap_y);
                // Adjust the position of the rectangle based on the overlap vector
                Vector2 move_amount = Vector2(coll_dir.x * overlap.x, coll_dir.y * overlap.y);
                if(GetBounciness() > 1.0f)
                {
                    MovableObj* parent = dynamic_cast<MovableObj*>(GetParent());
                    Vector2 vel = -parent->GetVel();
                    vel += Vector2(-move_amount.x * GetBounciness(), move_amount.y * GetBounciness());
                    parent->SetVel(vel);
                }
                SetPos(GetPos() + move_amount);
            }
        }
        break;
    }
}

void ColliderRect::CollisionPen(Collider* other)
{
    switch (other->GetType())
    {
    case CIRCLE:
        {
            ColliderCir* other_cir = dynamic_cast<ColliderCir*>(other);
            Vertex4 rect = GetRect();
            Vector2 circle_center = other_cir->GetPos();
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

            Vector2 collision_normal = circle_center - closest;
            float overlap = other_cir->GetRadius() - collision_normal.Distance();
            if (overlap > 0)
            {
                collision_normal.Normalize();
                SetPos(GetPos() - collision_normal * overlap);
            }
        }
        break;
    case RECTANGLE:
        {
            ColliderRect* other_rect = dynamic_cast<ColliderRect*>(other);
            Vertex4 rect = GetRect();
            Vertex4 other_rect_ = other_rect->GetRect();
            Vector2 collision_normal = other->GetPos() - GetPos();
            float overlap_x = abs(collision_normal.x) - GetParent()->GetScale().x / 2 - other->GetParent()->GetScale().x
                / 2;
            float overlap_y = abs(collision_normal.y) - GetParent()->GetScale().y / 2 - other->GetParent()->GetScale().y
                / 2;
            if (overlap_x < 0 || overlap_y < 0)
            {
                Vector2 coll_dir = collision_normal.Normalize();
                Vector2 overlap = Vector2(overlap_x, overlap_y);
                if (abs(coll_dir.y) > abs(coll_dir.x))
                {
                    if (coll_dir.y > 0.0f)
                    {
                        //go through bottom
                        SetPos(GetPos() + Vector2(
                            coll_dir.x * overlap.x,
                            GetParent()->GetScale().y / 2 + other->GetParent()->GetScale().y / 2));
                    }
                    else
                    {
                        Vector2 move_amount = Vector2(coll_dir.x * overlap.x, coll_dir.y * overlap.y);
                        SetPos(GetPos() + move_amount);
                    }
                }
                else
                {
                    Vector2 move_amount = Vector2(coll_dir.x * overlap.x, coll_dir.y * overlap.y);
                    SetPos(GetPos() + move_amount);
                }
            }
        }
        break;
    }
}
