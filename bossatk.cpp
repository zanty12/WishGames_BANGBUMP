#include "bossatk.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"

#define RANGE (SIZE_ * 4)                            //


//--------------火------------------------------------//
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
	SetType(OBJ_ATTACK);

}
void Boss_Thunder::Update()
{
	time_ += Time::GetDeltaTime();

	if (time_ > 4.0f)
		Discard();



	this->AddVel(GetVel());
}

//--------------風------------------------------------//
Boss_Wind::Boss_Wind(Vector2 pos)
	: MovableObj(pos - Vector2(0.0f, SIZE_ * 4), 0.0f, LoadTexture("data/texture/wall.png"), Vector2::Zero)
{
	SetScale(Vector2(SIZE_ * 4, SIZE_ * 4));
	speed_ = 96.0f * 8;
	boss_pos_ = pos;
	time_ = 0;
	SetType(OBJ_ATTACK);
	GetCollider()->SetBounciness(0.3f);
	float dt = Time::GetDeltaTime() < 1 ? Time::GetDeltaTime() : 0.0f; //初期化時のエラーを回避する
	SetVel(Vector2(-speed_ * dt, -speed_ * dt));
}
void Boss_Wind::Update()
{
	time_ += Time::GetDeltaTime();

	if (time_ > 4.0f)
		Discard();



	this->AddVel(GetVel());
}

/*float CalculationVertexLength(const Vector2& pos01, const Vector2& pos02)
{
	return sqrtf((pos01.x - pos02.x) * (pos01.x - pos02.x) + (pos01.y - pos02.y) * (pos01.y - pos02.y));
}

void ConvertToNomalizeVector(Vector2& out, Vector2 in)
{
	float distance = sqrtf((in.x * in.x) + (in.y * in.y));
	if (distance > 0.0f)
	{
		out.x = in.x / distance;
		out.y = in.y / distance;
	}
	else
	{
		out = Vector2(0.0f, 0.0f);
	}
}

float CalculationVectorLength(const Vector2& vec01)
{
	return sqrtf((vec01.x * vec01.x) + (vec01.y * vec01.y));

}

bool Boss_Wind::IsCollidingLineAndCircle(Line line, Circle circle)
{
	circle.Position = GetPos();

	Vector2 start_to_center = Vector2(circle.Position.x - line.Start.x, circle.Position.y - line.Start.y);
	Vector2 end_to_center = Vector2(circle.Position.y - line.End.y, circle.Position.y - line.End.y);
	Vector2 start_to_end = Vector2(line.End.x - line.Start.x, line.End.y - line.Start.y);
	Vector2 normal_start_to_end;

	// 単位ベクトル化する
	ConvertToNomalizeVector(normal_start_to_end, start_to_end);
	/*
		射影した線分の長さ
			始点と円の中心で外積を行う
			※始点 => 終点のベクトルは単位化しておく
	
	float distance_projection = start_to_center.x * normal_start_to_end.y - normal_start_to_end.x * start_to_center.y;

	// 射影の長さが半径よりも小さい
	if (fabs(distance_projection) < circle.Radius)
	{
		// 始点 => 終点と始点 => 円の中心の内積を計算する
		float dot01 = start_to_center.x * start_to_end.x + start_to_center.y * start_to_end.y;
		// 始点 => 終点と終点 => 円の中心の内積を計算する
		float dot02 = end_to_center.x * start_to_end.x + end_to_center.y * start_to_end.y;

		// 二つの内積の掛け算結果が0以下なら当たり
		if (dot01 * dot02 <= 0.0f)
		{
			return true;
		}
		/*
			上の条件から漏れた場合、円は線分上にはないので、
			始点 => 円の中心の長さか、終点 => 円の中心の長さが
			円の半径よりも短かったら当たり
		
		else if (CalculationVectorLength(start_to_center) < circle.Radius ||
			CalculationVectorLength(end_to_center) < circle.Radius)
		{
			return true;
		}
	}
	return false;
}
*/




//--------------水------------------------------------//
Boss_Water::Boss_Water(Vector2 pos)
	: MovableObj(pos - Vector2(0.0f, SIZE_ * 10), 0.0f, LoadTexture("data/texture/wall.png"), Vector2::Zero)
{
	SetScale(Vector2(SIZE_ * 3, SIZE_ * 10));
	boss_pos_ = pos;
	time_ = 0;
	SetType(OBJ_ATTACK);

}

void Boss_Water::Update()
{
	time_ += Time::GetDeltaTime();

	if (time_ > 3.0f)
		Discard();
}

