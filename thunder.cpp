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

bool Thunder::StickTrigger() {
	Vector2 stick = Input::GetStickLeft(0);
	Vector2 previousStick = Input::GetPreviousStickLeft(0);
	stick.y *= -1;
	previousStick.y *= -1;
	float distance = stick.Distance();


	if (responseMinStickDistance <= distance) {
		charge += distance;
		if (maxChage < charge) charge = maxChage;
	}

	if (previousStick != Vector2::Zero && stick == Vector2::Zero && responseMinOneFrameDistance < (stick - previousStick).Distance()) {
		return true;
	}

	return false;
}

Vector2 Thunder::Move() {
	Vector2 stick = Input::GetStickRight(0);
	Vector2 previousStick = Input::GetPreviousStickRight(0);
	stick.y *= -1;
	previousStick.y *= -1;
	float distance = stick.Distance();


	if (StickTrigger()) {
		Vector2 direction = -previousStick * charge * movePower;
		charge = 0.0f;
		return direction;
	}

	return player_->GetVel();
}

void Thunder::Action() {
	using namespace PHYSICS;
	Vector2 stick = Input::GetStickLeft(0);
	Vector2 previousStick = Input::GetPreviousStickLeft(0);

	if (StickTrigger()) {
		previousStick.y *= -1.0f;
		Vector2 direction = -previousStick * charge;
		charge = 0.0f;

		arrows_.push_back({ player_->GetPos(), direction });
	}

	auto arrow = arrows_.begin();
	for (int i = 0; i < arrows_.size(); i++) {
		float rad = atan2f(arrow->velocity.y, arrow->velocity.x);
		Vertex4 attackCollider = Vertex4(arrow->position, rad, arrowSize);

		DrawCollider(attackCollider, Color::Green);
		auto enemies = player_->GetMapMngr()->GetEnemyMngr()->GetEnemies();

		for (auto enemy : enemies) {
			if (enemy) {
				Vertex4 enemyCollider(enemy->GetPos(), 0.0f, enemy->GetScale());

				if (Collider2D::Touch(attackCollider, enemyCollider)) {
					enemy->Die();
					auto previousArrow = arrow;
					--arrow;
					arrows_.erase(previousArrow);
					break;
				}
			}
		}

		arrow->position += arrow->velocity * attackPower;
		arrow->velocity += Vector2::Down * arrowGravity;

		arrow++;
	}
}
