#include "wind.h"
#include "xinput.h"
#include "lib/collider2d.h"

bool Wind::StickTrigger(Vector2 stick, Vector2 previousStick) {
	float stickDistance = stick.Distance();
	float preStickDistance = previousStick.Distance();

	if (rotInputJudgeMin < MATH::Abs(Vector2::Cross(stick, previousStick)) &&
		0.9f < stickDistance * preStickDistance) {
		return true;
	}
	return false;
}

Vector2 Wind::Move(void) {
	Vector2 vel = player_->GetVel();
	Vector2 stick = Input::GetStickLeft(0);
	Vector2 previousStick = Input::GetPreviousStickLeft(0);

	// 回転のスピードを取得
	float rotSpeed = Vector2::Cross(stick, previousStick);

	// 移動中
	if (StickTrigger(stick, previousStick)) {
		power_ += rotSpeed * rotSpeed * rotInputFriction;
		if (maxPower_ < power_) power_ = maxPower_;

		vel = Vector2::Up * power_;
	}

	// 落下中の処理
	else if (0 < Vector2::Dot(Vector2::Down, vel)) {
		power_ *= friction_;

		vel.x = stick.x * 2.0f;
	}



	return vel;
}

void Wind::Action(void) {
	using namespace PHYSICS;
	Vector2 stick = Input::GetStickRight(0);
	Vector2 previousStick = Input::GetPreviousStickRight(0);
	isDraw = false;

	// 回転のスピードを取得
	float rotSpeed = Vector2::Cross(stick, previousStick);

	// 攻撃中
	if (StickTrigger(stick, previousStick)) {
		attackCollider = Vertex1(player_->GetPos(), attackRadius);

		isDraw = true;
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

void Wind::Draw(Vector2 offset) {
	if (isDraw) DrawCollider(attackCollider, Color::Green, offset);
}

void Wind::DebugMenu()
{
	ImGui::Begin("Wind");
	ImGui::Text("power: %.2f", power_);
	ImGui::SliderFloat("MaxPower", &maxPower_, 0.0f, 1000.0f);
	ImGui::SliderFloat("RotInputFriction", &rotInputFriction, 0.0f, 1.0f);
	ImGui::SliderFloat("RotInputJudgeMin", &rotInputJudgeMin, 0.0f, 1.0f);
	ImGui::SliderFloat("Friction", &friction_, 0.0f, 1.0f);
	ImGui::SliderFloat("AttackRadius", &attackRadius, 0.0f, 100.0f);
	ImGui::End();
}

void Wind::Gravity() {

}
