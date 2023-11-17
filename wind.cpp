#include "wind.h"
#include "xinput.h"

Vector2 Wind::Move(void) {
	Vector2 vel = player_.GetVel();
	Vector2 stick = Input::GetStickLeft(0);
	Vector2 preStick = Input::GetPreviousStickLeft(0);

	float stickDistance = stick.Distance();
	float preStickDistance = preStick.Distance();

	// 回転のスピードを取得
	float rotSpeed = Vector2::Cross(stick, preStick);

	// 移動中
	if (rotInputJudgeMin < MATH::Abs(Vector2::Cross(stick, preStick))&&
		0.9f < stickDistance * preStickDistance) {
		power_ += rotSpeed;
		if (maxPower_ < power_) power_ = maxPower_;		
	}
	
	// 落下中の処理
	else if (0 < Vector2::Dot(Vector2::Down, vel)) {
		power_ *= friction_;
	}

	return Vector2::Zero;
}

void Wind::Action(void) {


}