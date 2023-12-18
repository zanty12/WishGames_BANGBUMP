#include "collidercir.h"
#include "animator.h"
#include "game.h"

ColliderCir::ColliderCir(GameObject* parent, float radius): Collider(CIRCLE, parent)
{
    cir_.a = parent->GetPos();
    radius_ = radius;

    Game::GetCollMngr()->Add(this);

}

bool ColliderCir::Collide(Collider* other) const
{
    switch (other->GetType())
    {
    case CIRCLE:
        return Collider2D::Touch(cir_, dynamic_cast<ColliderCir*>(other)->GetCircle());
    case RECTANGLE:
        return Collider2D::Touch(cir_, dynamic_cast<ColliderRect*>(other)->GetRect());
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

void ColliderCir::OnCollision(Collider* other)
{
    GetCollision().push_back(other);
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
            GetParent()->SetPos(GetParent()->GetPos() - move);
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
                GetParent()->SetPos(GetParent()->GetPos() + collisionNormal * overlap);
            }
        }
        break;
    }
}
