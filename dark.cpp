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
	Vector2 velocity = player_->GetVel();

	if (Input::GetKey(0, Input::LThumb)) {
		if (velocity.y <= -maxSpeedFalling) {
			velocity.y = maxSpeedFalling;
			player_->SetVel(velocity);
		}
	}
	if (Input::GetKeyUp(0, Input::LThumb)) {
		Vector2 stick = Input::GetStickLeft(0);
		stick.y *= -1;
		Vector2 warpDirection = stick * warpDistance;
		warpPosition = player_->GetPos() + warpDirection;

		player_->SetPos(warpPosition);
	}
	else warpPosition = Vector2::Zero;


	return player_->GetVel();
};

void Dark::Action()
{
    using namespace PHYSICS;
    Vector2 stick = Input::GetStickRight(0);
    float distance = stick.Distance();



	float width = Graphical::GetWidth();
	float height = Graphical::GetHeight();
	Vector2 screenVertices[] = {
		Vector2(0.0f, 0.0f),
		Vector2(0.0f, height),
		Vector2(width, height),
		Vector2(width, 0.0f)
	};
	VertexN screenCollider(screenVertices, 4);					// スクリーンのコライダー
	DrawCollider(screenCollider, Color::Green);

	// 押し込む
	if (Input::GetKeyDown(0, Input::RThumb) && responseMinStickDistance < stick.Distance()) {
		attackDirection = stick.Normalize();
	}
	// 攻撃
	else if (Input::GetKey(0, Input::RThumb)) {
		auto enemies = player_->GetMapMngr()->GetEnemyMngr()->GetEnemies();
		Vertex4 attackCollider;									// レーザーのコライダー
		Vector2 normalize = attackDirection.Normalize();		// スティックの正規化ベクトル
		Vector2 maxAttackDirection = normalize * 1000.0f;		// 最大射程
		RayHit screenEndPoint;									// スクリーンにぶつかった場所を格納


		attackCollider = Vertex4(player_->GetPos(), player_->GetPos() + maxAttackDirection, attackWidthLength);
		DrawCollider(attackCollider, Color::Green);

		for (auto enemy : enemies) {
			if (enemy) {
				Vertex4 enemyCollider(enemy->GetPos(), 0.0f, enemy->GetScale());

				if (Collider2D::Touch(attackCollider, enemyCollider)) {
					enemy->Die();
				}
			}
		}
		//if (Collider2D::TouchLine(Vertex2(player_->GetPos(), maxAttackDirection), screenCollider, &screenEndPoint)) {
		//	// 画面までの距離
		//	float laserDistance = Vector2::Distance(player_->GetPos(), screenEndPoint.position);
		//	// レーザーのコライダー
		//	Vector2 laserEndPosition = player_->GetPos() + Vector2(normalize.x, -normalize.y) * laserDistance;
		//	attackCollider = Vertex4(player_->GetPos(), laserEndPosition, attackWidthLength);

		//	DrawCollider(Vertex1(screenEndPoint.position, 100), Color::White);

		//	DrawCollider(attackCollider, Color::Green);

		//	for (auto enemy : enemies) {
		//		if (enemy) {
		//			Vertex4 enemyCollider(enemy->GetPos(), 0.0f, enemy->GetScale());

		//			if (Collider2D::Touch(attackCollider, enemyCollider)) {
		//				enemy->Die();
		//			}
		//		}
		//	}
		//}
	}
};