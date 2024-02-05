#include "enemy1.h"
#include "Cell.h"
#include "MapMngr.h"
#include "playerattack.h"
#include "lib/collider2d.h"
#include "time.h"

bool CheckLength(Vector2 a, Vector2 b, float len);

void Enemy1::Update()
{
	//HPが0になったら消す
	if (GetHp() <= 0)
	{
		SetColor(Color(0, 0, 0, 0));    //透明にする
		SetType(OBJ_VOID);  //当たり判定を消す
		GetAnimator()->SetIsAnim(false);    //アニメーションをしない
		if (dead_effect_ == nullptr)
		{
			dead_effect_ = new EnemyDeadEffect(GetPos());   //エフェクト生成
			dead_effect_->SetScale(GetScale());
		}

		DropSkillOrb(GetPos(), SKILLORB_SIZE_TYPE_SMALL);
	}

	//消滅エフェクト
	if (dead_effect_)
	{
		//エフェクトが終了したらDiscard
		if (dead_effect_->EffectEnd())
		{
			delete dead_effect_;
			dead_effect_ = nullptr;
			Discard();
		}

		//エネミーには何もさせない
		return;
	}

	CollisionAction();

	GetAnimator()->SetIsAnim(true);

	if (!CheckLength(GetPos(), startPosition, (SIZE_ * 3.0f)))
	{
		startPosition = GetPos();
		SetVel(Vector2(GetVel().x * -1, GetVel().y));
		dir_ *= -1;
		GetAnimator()->Invert();    //反転
	}

	if (GetFlashing())
	{
		blinking();
	}

	this->AddVel(GetVel());
}

void Enemy1::CollisionAction(void)
{
	std::list<Collider*> collisions = GetCollider()->GetCollision();

	for (auto collision : collisions)
	{
		OBJECT_TYPE type = collision->GetParent()->GetType();
		switch (type)
		{
		case OBJ_PLAYER:
		{
			GameObject* obj = collision->GetParent();
			if (obj != nullptr)
			{
				//★テスト★
				//Player* player = dynamic_cast<Player*>(obj);
				//player->HpDown(300);

				Flashing();
			}

			break;
		}
		case OBJ_SPIKE:
			CollisionSpike();
			break;
		case OBJ_ATTACK:
			/*{
				PlayerAttack* attack = dynamic_cast<PlayerAttack*>(collision->GetParent());
				if(attack != nullptr)
					SetHp(GetHp() - attack->GetDamage());
			}*/
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
	dir_ *= -1; //反転させる

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