#include"dark.h"
#include"xinput.h"
#include"lib/collider2d.h"

Vector2 Dark::Move()
{



	if (GetPressStickLeft(0))
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
		player_.AddPos(stick * 20);
	}
	is_trigger_ = false;


};

void Dark::Action()
{
	

	using namespace PHYSICS;

	Vertex4 square(Vector2(0, 1), Vector2(1, 1), Vector2(1, -1), Vector2(0, -1));

	Vertex1 enemy(Vector2(0, 0),2.0f);

	bool isTouch = Collider2D::Touch(enemy, square);


};