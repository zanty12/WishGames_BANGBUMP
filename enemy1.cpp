#include "enemy1.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"

bool CheckLength(Vector2 a, Vector2 b, float len);

void Enemy1::Update()
{
    //HPが0になったら消す
    if (GetHp() <= 0)
    {
        GameObject::Discard();
        Die();
    }

	CollisionAction();

    GetAnimator()->SetIsAnim(true);

    if (!CheckLength(GetPos(), startPosition, (SIZE_ * 3.0f)))
    {
        startPosition = GetPos();
        SetVel(Vector2(GetVel().x * -1, GetVel().y));
        dir_ *= -1;
    }

    this->AddVel(GetVel());
}

SkillOrb* Enemy1::DropSkillOrb()
{
    if (GetDiscard() == false)
        return nullptr;

    switch (rand() % 4)
    {
    case 0:
        drop = SKILLORB_ATTRIBUTE_DESC::Fire();
        break;
    case 1:
        drop = SKILLORB_ATTRIBUTE_DESC::Dark();
        break;
    case 2:
        drop = SKILLORB_ATTRIBUTE_DESC::Wind();
        break;
    case 3:
        drop = SKILLORB_ATTRIBUTE_DESC::Thunder();
        break;
    default:
        break;
    }

    return new SkillOrb(GetPos(), drop, SKILLORB_SIZE_DESC::Small());
}

void Enemy1::CollisionAction(void)
{
	std::list<Collider*> collisions = GetCollider()->GetCollision();

	for (auto collision : collisions)
	{
		OBJECT_TYPE type = collision->GetParent()->GetType();
		switch (type)
		{
		case OBJ_SPIKE:
			CollisionSpike();
			break;
		default:
			break;
		}
	}
}

//================================================================================
// トゲに当たった時のアクション
//================================================================================
void Enemy1::CollisionSpike(void)
{

	dir_ = GetVel().Normalize();
	if (abs(GetVel().x) < 0.1f && abs(GetVel().y < 0.1f))
	{
		if (GetVel().x != 0.0f)
			SetVel(Vector2(0.0f, GetVel().y));
		if (GetVel().y != 0.0f)
			SetVel(Vector2(GetVel().x, 0.0f));

		//knockback_end_がバグるのでそのまま
		knockback_start_ = GetPos();
		knockback_end_ = GetPos();
		return;
	}
	dir_ *= -1;	//反転させる

    HpDown(6);

	knockback_distance_ = SIZE_;

	knockback_start_ = GetPos();
	knockback_end_ = GetPos() - (dir_ * knockback_distance_);
}

////================================================================================
//// エネミーに当たった時のアクション
////================================================================================
//void Player::CollisionEnemy(GameObject* obj)
//{
//	Enemy* enemy = dynamic_cast<Enemy*>(obj);
//
//	if (enemy == nullptr)
//	{
//		return;
//	}
//	if (invincibility_time_ < INVINCIBILITY_MAX_TIME_)
//	{
//		return;
//	}
//
//	invincibility_time_ = 0.0f;
//
//	dir_ = GetVel().Normalize();
//	if (abs(GetVel().x) < 0.1f && abs(GetVel().y < 0.1f))
//	{
//		dir_ = -enemy->GetVel().Normalize();
//	}
//	dir_ *= -1;	//反転させる
//
//	SkillPointDown(enemy->GetAtk());
//
//
//	switch (enemy->GetEnemyType())
//	{
//	case TYPE__KOOPA:
//		knockback_distance_ = SIZE_;
//		break;
//	case TYPE__HAMMERBRO:
//		knockback_distance_ = SIZE_;
//		break;
//	case TYPE__PHANTOM:
//		knockback_distance_ = SIZE_ * 3;
//		break;
//	default:
//		break;
//	}
//
//	knockback_start_ = GetPos();
//	knockback_end_ = GetPos() - (dir_ * knockback_distance_);
//
//}




bool CheckLength(Vector2 a, Vector2 b, float len)
{

    if (Vector2::Distance(a, b) < len)
    {
        return true;
    }

    return false;
}