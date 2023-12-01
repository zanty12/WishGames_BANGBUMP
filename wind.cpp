#include "wind.h"
#include "xinput.h"
#include "lib/collider2d.h"

Vector2 Wind::Move(void) {
	Vector2 vel = player_->GetVel();
	Vector2 stick = Input::GetStickLeft(0);
	Vector2 preStick = Input::GetPreviousStickLeft(0);

	float stickDistance = stick.Distance();
	float preStickDistance = preStick.Distance();

	// 回転のスピードを取得
	float rotSpeed = Vector2::Cross(stick, preStick);

	// 移動中
	if (rotInputJudgeMin < MATH::Abs(Vector2::Cross(stick, preStick)) &&
		0.9f < stickDistance * preStickDistance) {
		power_ += rotSpeed * rotSpeed * rotInputFriction;
		if (maxPower_ < power_) power_ = maxPower_;
	}

	// 落下中の処理
	else if (0 < Vector2::Dot(Vector2::Down, vel)) {
		power_ *= friction_;
	}

	vel += Vector2::Up * power_;
	player_->SetVel(vel);
	return vel;
}

void Wind::Action(void) {
	using namespace PHYSICS;
	Vector2 stick = Input::GetStickRight(0);
	Vector2 preStick = Input::GetPreviousStickRight(0);

	float stickDistance = stick.Distance();
	float preStickDistance = preStick.Distance();

	// 回転のスピードを取得
	float rotSpeed = Vector2::Cross(stick, preStick);

	// 攻撃中
	if (rotInputJudgeMin < MATH::Abs(Vector2::Cross(stick, preStick)) &&
		0.9f < stickDistance * preStickDistance) {
		Vertex1 attackCollider = Vertex1(player_->GetPos(), attackRadius);

		DrawCollider(attackCollider, Color::Green);
		auto enemies = player_->GetMapMngr()->GetEnemyMngr()->GetEnemies();

		for (auto enemy : enemies) {
			if (enemy) {
				Vertex4 enemyCollider(enemy->GetPos(), 0.0f, enemy->GetScale());

				if (Collider2D::Touch(attackCollider, enemyCollider)) {
					enemy->Die();
				}
			}
		}
	}
}