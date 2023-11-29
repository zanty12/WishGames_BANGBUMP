//--------------------------------------------------------------------------------
// 
// ワープ[fark.cpp]
// 
// 作成者 趙潤博
// 
// 最終更新日	2023/11/22
// 
//--------------------------------------------------------------------------------
#include"dark.h"
#include"xinput.h"
#include"lib/collider2d.h"

Vector2 Dark::Move()
{


	if (Input::GetKey(0,Input::LThumb))
	{
		if (!is_press_)
		{
			is_trigger_ = true;
		}

		is_press_ = true;
	}
	else
	{
		is_press_ = false;
	}

	if (is_trigger_)
	{
		Vector2 vel = player_.GetVel();
		Vector2 stick = Input::GetStickLeft(0);
		Vector2 pos = player_.GetPos();
		player_.SetPos(pos + stick * 20);
	}
	is_trigger_ = false;

	return player_.GetPos();
};

void Dark::Action()
{
	if (Input::GetKey(0,Input::RThumb))
	{
		if (!is_press_)
		{
			is_trigger_ = true;
		}

		is_press_ = true;

	}
	else
	{
		is_press_ = false;
	}

	if (is_trigger_)
	{
		player_.SetVel(Vector2(0, 0));
	}
	is_trigger_ = false;

	Vector2 stick = Input::GetStickRight(0);

	float rad = atan2f(stick.y, stick.x);

	using namespace PHYSICS;

	Vertex4 square(Vector2(0, 1), Vector2(1, 1), Vector2(1, -1), Vector2(0, -1));

	square.Rotate(rad);



	//bool isTouch = Collider2D::Touch(enemy, square);


};