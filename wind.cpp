#include "wind.h"
#include "xinput.h"
#include "lib/collider2d.h"

Vector2 Wind::Move(void) {
	Vector2 vel = player_->GetVel();
	Vector2 stick = Input::GetStickLeft(0);
	Vector2 preStick = Input::GetPreviousStickLeft(0);

	float stickDistance = stick.Distance();
	float preStickDistance = preStick.Distance();

	// ��]�̃X�s�[�h���擾
	float rotSpeed = Vector2::Cross(stick, preStick);

	// �ړ���
	if (rotInputJudgeMin < MATH::Abs(Vector2::Cross(stick, preStick)) &&
		0.9f < stickDistance * preStickDistance) {
		power_ += rotSpeed * rotSpeed * rotInputFriction;
		if (maxPower_ < power_) power_ = maxPower_;
	}

	// �������̏���
	else if (0 < Vector2::Dot(Vector2::Down, vel)) {
		power_ *= friction_;
	}

	vel += Vector2::Up * power_;
	player_->AddVel(vel);
	return vel;
}

void Wind::Action(void) {
	using namespace PHYSICS;


}