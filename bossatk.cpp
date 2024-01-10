#include "bossatk.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"

//--------------âŒ------------------------------------//
Boss_Fire::Boss_Fire(Vector2 pos)
	: MovableObj(pos - Vector2(SIZE_ * 2, SIZE_ * 2), 0.0f, LoadTexture("data/texture/wall.png"), Vector2::Zero)
{
	SetRot(3.14f / 2);
	SetScale(Vector2(SIZE_ * 2, SIZE_ * 2));
	cheak_ = false;
	range_ = 3.14f / 6 + 3.14f / 2;
	speed_ = 0.004f;
	boss_pos_ = pos;
	time_ = 0;
}
void Boss_Fire::Update()
{
	time_ += Time::GetDeltaTime();

	Vector2 pos_;
	pos_.x = boss_pos_.x + cosf(GetRot()) * GetScale().x * 3;
	pos_.y = boss_pos_.y + -sinf(GetRot()) * GetScale().y * 3;

	SetPos(pos_);

	if (time_ > 2.0f)
		Discard();

	if (abs(GetRot()-3.14f / 2) > abs(3.14f / 6))
	{
		cheak_ = true;
	}
	else
	{
		cheak_ = false;
	}

	if (cheak_ == false)
	{
		float rot = GetRot() + speed_;
	}
	else
	{
		if (speed_ < 0)
		{
			range_ = -(3.14f / 6) + 3.14f / 2;
		}
		else
		{
			range_ = 3.14f / 6 + 3.14f / 2;
		}
		speed_ *= -1;
	}
	
	SetRot(GetRot() + speed_);


}


//--------------óã-----------------------------------//
Boss_Thunder::Boss_Thunder(Vector2 pos)
	: MovableObj(pos, 0.0f, 0.0f, Vector2::Zero)
{
	SetScale(Vector2(SIZE_ * 2, SIZE_ * 2));


}
void Boss_Thunder::Update()
{

}

//--------------ïó------------------------------------//
Boss_Wind::Boss_Wind(Vector2 pos)
	: MovableObj(pos, 0.0f, LoadTexture("data/texture/wall.png"), Vector2::Zero)
{
	SetScale(Vector2(SIZE_, SIZE_/*SIZE_ * 2, SIZE_ * 2*/));
	speed_ = 96.0f;
	boss_pos_ = pos;
	time_ = 0;
}
void Boss_Wind::Update()
{
	time_ += Time::GetDeltaTime();
	float dt = Time::GetDeltaTime() < 1 ? Time::GetDeltaTime() : 0.0f; //èâä˙âªéûÇÃÉGÉâÅ[ÇâÒîÇ∑ÇÈ

	Vector2 pos_;
	pos_.x = boss_pos_.x;
	pos_.y = boss_pos_.y  -(SIZE_ * 7);
	SetPos(pos_);

	if (time_ > 4.0f)
		Discard();

	SetVel(Vector2(-speed_ * dt, -speed_ * dt));


	//CollosionAction();
	

	this->AddVel(GetVel());
}
void Boss_Wind::CollosionAction(void)
{
	std::list<Collider*> collisions = GetCollider()->GetCollision();

	/*for (auto collision : collisions)
	{
		OBJECT_TYPE type = collision->GetParent()->GetType();
		switch (type)
		{
		case OBJ_SOLID:
			switch ()
			{
			default:
				break;
			}
			break;
		default:
			break;
		}
	}*/
}

//--------------êÖ------------------------------------//
Boss_Water::Boss_Water(Vector2 pos)
	: MovableObj(pos, 0.0f, 0.0f, Vector2::Zero)
{
	SetScale(Vector2(SIZE_ * 2, SIZE_ * 2));


}

void Boss_Water::Update()
{

}

