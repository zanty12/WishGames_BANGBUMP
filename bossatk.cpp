#include "bossatk.h"
#include "Cell.h"
#include "MapMngr.h"
#include "lib/collider2d.h"
#include "time.h"

#define RANGE (SIZE_ * 4)                            //


//--------------��------------------------------------//
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


//--------------��-----------------------------------//
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

//--------------��------------------------------------//
Boss_Wind::Boss_Wind(Vector2 pos)
	: MovableObj(pos - Vector2(0.0f, SIZE_ * 4), 0.0f, LoadTexture("data/texture/wall.png"), Vector2::Zero)
{
	SetScale(Vector2(SIZE_ * 4, SIZE_ * 4));
	speed_ = 96.0f * 8;
	boss_pos_ = pos;
	time_ = 0;
	SetType(OBJ_ATTACK);
	GetCollider()->SetBounciness(0.3f);
	float dt = Time::GetDeltaTime() < 1 ? Time::GetDeltaTime() : 0.0f; //���������̃G���[���������
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

	// �P�ʃx�N�g��������
	ConvertToNomalizeVector(normal_start_to_end, start_to_end);
	/*
		�ˉe���������̒���
			�n�_�Ɖ~�̒��S�ŊO�ς��s��
			���n�_ => �I�_�̃x�N�g���͒P�ʉ����Ă���
	
	float distance_projection = start_to_center.x * normal_start_to_end.y - normal_start_to_end.x * start_to_center.y;

	// �ˉe�̒��������a����������
	if (fabs(distance_projection) < circle.Radius)
	{
		// �n�_ => �I�_�Ǝn�_ => �~�̒��S�̓��ς��v�Z����
		float dot01 = start_to_center.x * start_to_end.x + start_to_center.y * start_to_end.y;
		// �n�_ => �I�_�ƏI�_ => �~�̒��S�̓��ς��v�Z����
		float dot02 = end_to_center.x * start_to_end.x + end_to_center.y * start_to_end.y;

		// ��̓��ς̊|���Z���ʂ�0�ȉ��Ȃ瓖����
		if (dot01 * dot02 <= 0.0f)
		{
			return true;
		}
		/*
			��̏�������R�ꂽ�ꍇ�A�~�͐�����ɂ͂Ȃ��̂ŁA
			�n�_ => �~�̒��S�̒������A�I�_ => �~�̒��S�̒�����
			�~�̔��a�����Z�������瓖����
		
		else if (CalculationVectorLength(start_to_center) < circle.Radius ||
			CalculationVectorLength(end_to_center) < circle.Radius)
		{
			return true;
		}
	}
	return false;
}
*/




//--------------��------------------------------------//
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

