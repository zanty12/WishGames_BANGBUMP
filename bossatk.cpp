#include "bossatk.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"



#define RANGE (SIZE_ * 4)                            //


//--------------火------------------------------------//
Boss_Fire::Boss_Fire(Vector2 pos)
	: MovableObj(pos - Vector2(SIZE_ * 2, SIZE_ * 2), 0.0f, LoadTexture("data/texture/fire_.png"), Vector2::Zero)
{
	SetRot(3.14f / 2);
	SetScale(Vector2(SIZE_ * 2, SIZE_ * 2));
	cheak_ = false;
	range_ = 3.14f / 6 + 3.14f / 2;
	speed_ = 0.008f;
	boss_pos_ = pos;
	time_ = 0;
	SetType(OBJ_ATTACK);
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


//--------------雷-----------------------------------//
Boss_Thunder::Boss_Thunder(Vector2 pos)
	: MovableObj(pos - Vector2(0.0f, SIZE_ * 2), 0.0f, LoadTexture("data/texture/wall.png"), Vector2::Zero)
{
	SetScale(Vector2(SIZE_ * 2, SIZE_ * 2));
	boss_pos_ = pos;
	time_ = 0;
	speed_ = 96.0f * 6;
	SetType(OBJ_ATTACK);
	float dt = Time::GetDeltaTime() < 1 ? Time::GetDeltaTime() : 0.0f; //初期化時のエラーを回避する
	SetVel(Vector2(-speed_ * dt, -speed_ * dt));
}
void Boss_Thunder::Update()
{
	time_ += Time::GetDeltaTime();

	if (time_ > 1.0f)
		Discard();	

	this->AddVel(GetVel());
}

//--------------風------------------------------------//
Boss_Wind::Boss_Wind(Vector2 pos)
	: MovableObj(pos - Vector2(0.0f, SIZE_ * 4), 0.0f, LoadTexture("data/texture/wind.png"), Vector2::Zero)
{
	SetScale(Vector2(SIZE_ * 3, SIZE_ * 3));
	speed_ = 96.0f * 6;
	boss_pos_ = pos;
	time_ = 0;
	SetType(OBJ_ATTACK);
	//GetCollider()->SetBounciness(0.3f);
	float dt = Time::GetDeltaTime() < 1 ? Time::GetDeltaTime() : 0.0f; //初期化時のエラーを回避する
	SetVel(Vector2(-speed_ * dt, -speed_ * dt));
	
}
void Boss_Wind::Update()
{
	time_ += Time::GetDeltaTime();
	std::list<Collider*> collisions = GetCollider()->GetCollision();

	for (auto collision : collisions)
	{
		OBJECT_TYPE type = collision->GetParent()->GetType();
		
		Vector2 wallup, walldown, walllaft, wallright;
		Vector2 wallpos;
		wallpos = collision->GetPos();

		wallup = wallpos - GetPos();
		walldown = wallpos - GetPos();
		walllaft = wallpos - GetPos();
		wallright = wallpos - GetPos();

		wallup.y += SIZE_ / 2;
		walldown.y -= SIZE_ / 2;
		walllaft.x -= SIZE_ / 2;
		wallright.x += SIZE_ / 2;

		float up, down, laft, right;

		
		up = wallup.Distance();
		down = walldown.Distance();
		laft = walllaft.Distance();
		right = wallright.Distance();



		switch (type)
		{
		case OBJ_SOLID:
			
			if (up < down && up < laft && up < right)
			{
				if (GetVel().y < 0)
				{
					SetVel(Vector2(GetVel().x, GetVel().y * -1));
					//SetVel(Reflection((collision->GetPos().x + 48.0f), (collision->GetPos().y + 48.0f), (collision->GetPos().x - 48.0f), (collision->GetPos().y + 48.0f), GetVel().x, GetVel().y));
					//上
				}
			}
			else if (down < laft && down < right)
			{
				if (GetVel().y > 0)
				{
					SetVel(Vector2(GetVel().x, GetVel().y * -1));
					//SetVel(Reflection((collision->GetPos().x + 48.0f), (collision->GetPos().y - 48.0f), (collision->GetPos().x - 48.0f), (collision->GetPos().y - 48.0f), GetVel().x, GetVel().y));
					//下
				}
			}
			else if (laft < right)
			{
				if (GetVel().x > 0)
				{
					SetVel(Vector2(GetVel().x * -1, GetVel().y));
					//SetVel(Reflection((collision->GetPos().x - 48.0f), (collision->GetPos().y + 48.0f), (collision->GetPos().x - 48.0f), (collision->GetPos().y - 48.0f), GetVel().x, GetVel().y));
					//左
				}
			}
			else
			{
				if (GetVel().x < 0)
				{
					SetVel(Vector2(GetVel().x * -1, GetVel().y));
					//SetVel(Reflection((collision->GetPos().x + 48.0f), (collision->GetPos().y - 48.0f), (collision->GetPos().x + 48.0f), (collision->GetPos().y + 48.0f), -GetVel().x, GetVel().y));
					//右
				}
			}
			break;

		default:
			break;
		}
	}

	if (time_ > 3.0f)
		Discard();


	this->AddVel(GetVel());
}

Vector2 Boss_Wind::Reflection(float spx, float spy, float epx, float epy, float velx, float vely)
{
	Sp.x = spx;
	Sp.y = spy;
	Ep.x = epx;
	Ep.y = epy;
	v = Ep - Sp;
	N.x = -v.y;
	N.y = v.x;
	/*N.x = v.x;
	N.y = -v.y;*/
	N = N.Normalize();
	vel_.x = velx;
	vel_.y = vely;
	float T = (-vel_.x * N.x) + (-vel_.y * N.y) * 2;
	Vector2 reflection_vel_;
	reflection_vel_ = T * N;
	
	Vector2 reflection_ = vel_ + reflection_vel_;

	return reflection_;

}


//--------------水------------------------------------//
Boss_Water::Boss_Water(Vector2 pos)
	: MovableObj(pos - Vector2(0.0f, SIZE_ * 10), 0.0f, LoadTexture("data/texture/water.png"), Vector2::Zero)
{
	SetScale(Vector2(SIZE_ * 3, SIZE_ * 10));
	boss_pos_ = pos;
	time_ = 0;
	SetType(OBJ_ATTACK);

}

void Boss_Water::Update()
{
	time_ += Time::GetDeltaTime();

	if (time_ > 2.5f)
		Discard();
}

