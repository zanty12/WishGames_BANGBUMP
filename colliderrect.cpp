#include "colliderrect.h"

#include "game.h"

ColliderRect::ColliderRect(GameObject* parent) : Collider(RECTANGLE, parent)
{
    const Vertex4 rect = Vertex4(Vector2(parent->GetPos().x - parent->GetScale().x / 2, parent->GetPos().y + parent->GetScale().y / 2),
                                 Vector2(parent->GetPos().x + parent->GetScale().x / 2, parent->GetPos().y + parent->GetScale().y / 2),
                                 Vector2(parent->GetPos().x + parent->GetScale().x / 2, parent->GetPos().y - parent->GetScale().y / 2),
                                 Vector2(parent->GetPos().x - parent->GetScale().x / 2, parent->GetPos().y - parent->GetScale().y / 2));
    rect_ = rect;
    Game::GetCollMngr()->Add(this);
}

bool ColliderRect::Collide(Collider* other) const
{
    switch(other->GetType())
    {
    case CIRCLE:
        return Collider2D::Touch(rect_, dynamic_cast<ColliderCir*>(other)->GetCircle());
    case RECTANGLE:
        return Collider2D::Touch(rect_, dynamic_cast<ColliderRect*>(other)->GetRect());
    default:
        return false;
    }

}

void ColliderRect::Update()
{
    if(GetPos() != GetParent()->GetPos())
    {
        SetPos(GetParent()->GetPos());
        rect_.a = Vector2(GetPos().x - GetParent()->GetScale().x / 2, GetPos().y + GetParent()->GetScale().y / 2);
        rect_.b = Vector2(GetPos().x + GetParent()->GetScale().x / 2, GetPos().y + GetParent()->GetScale().y / 2);
        rect_.c = Vector2(GetPos().x + GetParent()->GetScale().x / 2, GetPos().y - GetParent()->GetScale().y / 2);
        rect_.d = Vector2(GetPos().x - GetParent()->GetScale().x / 2, GetPos().y - GetParent()->GetScale().y / 2);
    }
}

void ColliderRect::OnCollision(Collider* other)
{
    GetCollision().push_back(other);
    switch(other->GetType())
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
                float t = Vector2::Dot((circle_center- rect_side_start), side_dir);
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
                GetParent()->SetPos(GetParent()->GetPos() - collision_normal * overlap);
            }
        }
        break;
    case RECTANGLE:
        {
            ColliderRect* other_rect = dynamic_cast<ColliderRect*>(other);
            Vertex4 rect = GetRect();
            Vertex4 other_rect_ = other_rect->GetRect();
            Vector2 collision_normal = other->GetPos() - GetPos();
            float overlap_x = GetParent()->GetScale().x / 2 + other->GetParent()->GetScale().x / 2 - abs(collision_normal.x);
            float overlap_y = GetParent()->GetScale().y / 2 + other->GetParent()->GetScale().y / 2 - abs(collision_normal.y);
            if (overlap_x > 0 && overlap_y > 0)
            {
                if (overlap_x < overlap_y)
                {
                    if (collision_normal.x < 0)
                    {
                        GetParent()->SetPos(GetParent()->GetPos() - Vector2(overlap_x, 0));
                    }
                    else
                    {
                        GetParent()->SetPos(GetParent()->GetPos() + Vector2(overlap_x, 0));
                    }
                }
                else
                {
                    if (collision_normal.y < 0)
                    {
                        GetParent()->SetPos(GetParent()->GetPos() - Vector2(0, overlap_y));
                    }
                    else
                    {
                        GetParent()->SetPos(GetParent()->GetPos() + Vector2(0, overlap_y));
                    }
                }
            }
        }

        break;
    }
}