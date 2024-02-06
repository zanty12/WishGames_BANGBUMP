#include "colliderrect.h"

#include <libavutil/mathematics.h>

#include "bossatk.h"
#include "gamebase.h"

ColliderRect::ColliderRect(GameObject* parent, bool movable) : Collider(RECTANGLE, parent, movable)
{
    rect_ = Vertex4(
        Vector2(parent->GetPos().x - parent->GetScale().x / 2, parent->GetPos().y + parent->GetScale().y / 2),
        Vector2(parent->GetPos().x + parent->GetScale().x / 2, parent->GetPos().y + parent->GetScale().y / 2),
        Vector2(parent->GetPos().x + parent->GetScale().x / 2, parent->GetPos().y - parent->GetScale().y / 2),
        Vector2(parent->GetPos().x - parent->GetScale().x / 2, parent->GetPos().y - parent->GetScale().y / 2));
    GameBase::GetCollMngr()->Add(this);
}

void ColliderRect::SetPos(Vector2 pos)
{
    Collider::SetPos(pos);
    rect_.a = Vector2(GetPos().x - GetParent()->GetScale().x / 2, GetPos().y + GetParent()->GetScale().y / 2);
    rect_.b = Vector2(GetPos().x + GetParent()->GetScale().x / 2, GetPos().y + GetParent()->GetScale().y / 2);
    rect_.c = Vector2(GetPos().x + GetParent()->GetScale().x / 2, GetPos().y - GetParent()->GetScale().y / 2);
    rect_.d = Vector2(GetPos().x - GetParent()->GetScale().x / 2, GetPos().y - GetParent()->GetScale().y / 2);
}

bool ColliderRect::Collide(Collider* other)
{
    switch (other->GetType())
    {
    case CIRCLE:
        return Collider2D::Touch(rect_, dynamic_cast<ColliderCir*>(other)->GetCircle());
    case RECTANGLE:
        //return Collider2D::Touch(rect_, dynamic_cast<ColliderRect*>(other)->GetRect());
        /*{
            //bounding box
            Vertex4 other_rect = dynamic_cast<ColliderRect*>(other)->GetRect();
            return (rect_.a.x < other_rect.b.x && rect_.b.x > other_rect.a.x &&
                rect_.a.y > other_rect.c.y && rect_.c.y < other_rect.a.y);
        }*/
        //SAT
        return CheckIntersect(rect_, dynamic_cast<ColliderRect*>(other)->GetRect());
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

    //Žè”²‚«‚Å‚«‚È‚©‚Á‚½
    //rect_.Rotate(GetParent()->GetRot());
    //apply rotation around center of collider
    Vector2 center = GetPos(); // Center of the rectangle
    // Subtract the center from each vertex (translate to origin)
    rect_.a -= center;
    rect_.b -= center;
    rect_.c -= center;
    rect_.d -= center;
    // Rotate each vertex around the origin
    float rot = GetParent()->GetRot();
    rect_.Rotate(-rot);

    // Add the center back to each vertex (translate back)
    rect_.a += center;
    rect_.b += center;
    rect_.c += center;
    rect_.d += center;
}

void ColliderRect::CollisionInteract()
{
    if (GetParent()->GetType() == OBJ_ATTACK || GetParent()->GetType() == OBJ_VOID || GetParent()->GetType() == OBJ_BULLET)
        return;
    for (auto& other : collision_)
    {
        switch (other->GetParent()->GetType())
        {
        case OBJ_SOLID:
            CollisionSolid(other);
            break;
        case OBJ_PLAYER:
            CollisionSolid(other);
            break;
        case OBJ_PENETRABLE:
            {
                if (GetBounciness() > 0.0f)
                    CollisionSolid(other);
                else
                    CollisionPen(other);
            }
            break;
        case OBJ_VOID:
            break;
        case OBJ_ITEM:
            break;
        case OBJ_ATTACK:
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
                //come from the side
                if (abs(coll_dir.x) > abs(coll_dir.y))
                {
                    overlap_y = 0;
                }
                //come from the top or bottom
                else
                {
                    overlap_x = 0;
                }
                Vector2 overlap = Vector2(overlap_x, overlap_y);
                // Adjust the position of the rectangle based on the overlap vector
                Vector2 move_amount = Vector2(coll_dir.x * overlap.x, coll_dir.y * overlap.y);
                if (GetBounciness() > 0.0f)
                {
                    MovableObj* parent = dynamic_cast<MovableObj*>(GetParent());
                    if (parent != nullptr)
                    {
                        Vector2 vel = parent->GetVel() * GetBounciness();
                        //if the object is moving towards the collision, bounce it
                        if (Vector2::Dot(vel, coll_dir) > 0)
                        {
                            if (overlap_x != 0.0f)
                                vel.x = -vel.x;
                            if (overlap_y != 0.0f)
                                vel.y = -vel.y;
                        }
                        //vel += Vector2(-move_amount.x * GetBounciness(), move_amount.y * GetBounciness());
                        parent->SetVel(vel);
                    }
                }
                //if other is heavier or not movable, move self
                if (GetWeight() > other->GetWeight() || !other->GetIsMovable())
                    SetPos(GetPos() + move_amount);
                else
                    //if other is lighter, move other
                    other->SetPos(other->GetPos() - move_amount);
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
                //from bottom or top
                if (abs(coll_dir.y) > abs(coll_dir.x))
                {
                    overlap.x = 0;
                    //bottom
                    if (coll_dir.y > 0.0f)
                    {
                        //go through bottom
                        SetPos(GetPos() + Vector2(
                            coll_dir.x * overlap.x,
                            GetParent()->GetScale().y / 2 + other->GetParent()->GetScale().y / 2));
                    }
                    //top
                    else
                    {
                        overlap.y = 0;
                        Vector2 move_amount = Vector2(coll_dir.x * overlap.x, coll_dir.y * overlap.y);
                        SetPos(GetPos() + move_amount);
                    }
                }
                //from side
                else
                {
                    overlap.y = 0;
                    Vector2 move_amount = Vector2(coll_dir.x * overlap.x, coll_dir.y * overlap.y);
                    SetPos(GetPos() + move_amount);
                }
            }
        }
        break;
    }
}

void ColliderRect::Draw(int tex, Vector2 offset)
{
    const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
    const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
    Vector2 uvs[4] = {
        Vector2(0.0f, 0.0f),
        Vector2(0.0f, 1.0f),
        Vector2(1.0f, 0.0f),
        Vector2(1.0f, 1.0f),
    };
    Vector2 rect[4] = {
        (rect_.a - offset),
        (rect_.d - offset),
        (rect_.b - offset),
        (rect_.c - offset)
    };
    rect[0].y = Graphical::GetHeight() - rect[0].y;
    rect[1].y = Graphical::GetHeight() - rect[1].y;
    rect[2].y = Graphical::GetHeight() - rect[2].y;
    rect[3].y = Graphical::GetHeight() - rect[3].y;
    /*rect[0] *= scale_x;
    rect[1] *= scale_x;
    rect[2] *= scale_x;
    rect[3] *= scale_x;*/
    DrawSprite(tex, rect, uvs, Color(0, 1.0f, 1.0f, 0.5f));
}

bool ColliderRect::CheckIntersect(Vertex4 rect, Vertex4 other)
{
    // Calculate axes (normals of the edges)
    Vector2 axes[4] = {
        (rect.b - rect.a).Normal(),
        (rect.b - rect.c).Normal(),
        (other.b - other.a).Normal(),
        (other.b - other.c).Normal()
    };

    // For each axis
    for (int i = 0; i < 4; i++)
    {
        // Project the rectangles onto the axis
        float min_rect = Vector2::Dot(rect.a, axes[i]);
        float max_rect = min_rect;
        for (int j = 1; j < 4; j++)
        {
            float projection = Vector2::Dot(rect.v[j], axes[i]);
            min_rect = min(min_rect, projection);
            max_rect = max(max_rect, projection);
        }

        float min_other_rect = Vector2::Dot(other.a, axes[i]);
        float max_other_rect = min_other_rect;
        for (int j = 1; j < 4; j++)
        {
            float projection = Vector2::Dot(other.v[j], axes[i]);
            min_other_rect = min(min_other_rect, projection);
            max_other_rect = max(max_other_rect, projection);
        }

        // Check for overlap
        if (max_rect < min_other_rect || max_other_rect < min_rect)
        {
            // No overlap
            return false;
        }
    }

    // Overlap on all axes, the rectangles intersect
    return true;
}
