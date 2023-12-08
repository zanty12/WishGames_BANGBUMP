//--------------------------------------------------------------------------------
// 
// サンダー[thunder.cpp]
// 
// 作成者 趙潤博
// 
// 最終更新日	2023/11/22
// 
//--------------------------------------------------------------------------------
#include "thunder.h"
#include"xinput.h"
#include"lib/collider2d.h"

bool Thunder::StickTrigger(Vector2 stick, Vector2 previousStick) {
	stick.y *= -1;
	previousStick.y *= -1;
	float distance = stick.Distance();
	float previousDistance = previousStick.Distance();

	if (responseMinStickDistance <= distance) {
		charge += distance;
		if (maxCharge < charge) charge = maxCharge;
	}

	if (distance < responseMinStickDistance && responseMinStickDistance < previousDistance &&
		responseMinOneFrameDistance < (stick - previousStick).Distance()) {
		return true;
	}

	return false;
}

Vector2 Thunder::Move() {
	Vector2 stick = Input::GetStickLeft(0);
	Vector2 previousStick = Input::GetPreviousStickLeft(0);
	stick.y *= -1;
	previousStick.y *= -1;
	float distance = stick.Distance();


	if (StickTrigger(stick, previousStick)) {
		Vector2 direction = -previousStick * 10 * movePower;
		charge = 0.0f;

		return direction;
	}
	else if(stick!=Vector2::Zero)
	{
		return Vector2::Zero;
	}

	return player_->GetVel();
}

void Thunder::Action() {
	using namespace PHYSICS;
	Vector2 stick = Input::GetStickRight(0);
	Vector2 previousStick = Input::GetPreviousStickRight(0);

	if (StickTrigger(stick, previousStick)) {
		previousStick.y *= -1.0f;
		Vector2 direction = -previousStick * charge;
		charge = 0.0f;

		for (int i = 0; i < _countof(arrows_); i++)
			if (!arrows_[i].isVisible) arrows_[i] = { player_->GetPos(), direction, true };
	}

	for (int i = 0; i < _countof(arrows_); i++) {
		auto &arrow = arrows_[i];
		if (!arrow.isVisible) continue;


		auto enemies = player_->GetMapMngr()->GetEnemyMngr()->GetEnemies();

		if (3000 < Vector2::Distance(player_->GetPos(), arrow.position)) {
			arrows_[i].isVisible = false;
			continue;
		}
		for (auto enemy : enemies) {
			if (enemy) {
				Vertex4 enemyCollider(enemy->GetPos(), 0.0f, enemy->GetScale());
				float rad = atan2f(arrow.velocity.y, arrow.velocity.x);
				auto attackCollider = PHYSICS::Vertex4(arrow.position, rad, arrowSize);
				if (Collider2D::Touch(attackCollider, enemyCollider)) {
					enemy->Die();
					arrows_[i].isVisible = false;
					break;
				}
			}
		}

		arrow.position += arrow.velocity * attackPower;
		arrow.velocity += Vector2::Down * arrowGravity;
	}
}

void Thunder::Draw(Vector2 offset) {

	for (int i = 0; i < 3; i++) {
		auto arrow = arrows_[i];
		float rad = atan2f(arrow.velocity.y, arrow.velocity.x);
		if (arrow.isVisible) {
			auto attackCollider = PHYSICS::Vertex4(arrow.position, rad, arrowSize);
			DrawCollider(attackCollider, Color::Green, offset);
		}
	}
}

void Thunder::DebugMenu()
{
	ImGui::Begin("Thunder");
	ImGui::Text("charge:%f", charge);
	ImGui::SliderFloat("maxCharge", &maxCharge, 0.0f, 100.0f);
	ImGui::SliderFloat("movePower", &movePower, 0.0f, 5.0f);
	ImGui::SliderFloat("attackPower", &attackPower, 0.0f, 10.0f);
	ImGui::End();
}
