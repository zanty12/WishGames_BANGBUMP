#include "multi_last_result_mode.h"
#include "move_scene_anim.h"

void MultiPlayLastResultModeClientSide::Draw(RESPONSE_PLAYER &players, Vector2 offset) {
	AllMoveScene.Move(Color::White * 0.0f);
	UIMoveScene.Move(Color::White * 0.0f);

	// 動画の描画
	video->Update();
	video->DrawAsResource(Color(0.5f, 0.5f, 0.5f));

	if (ranking.size() == 0) {
		ranking = players.clients;
		ranking.sort([](CLIENT_DATA_CLIENT_SIDE &a, CLIENT_DATA_CLIENT_SIDE &b) {
			return a.skillPoint > b.skillPoint;
			}
		);
	}



	Vector2 screen = Vector2(Graphical::GetWidth(), Graphical::GetHeight());
	Vector2 center = screen * 0.5f;
	Vector2 sclBar = Vector2(1899, 324) * 0.5f;
	Vector2 victory = Vector2(screen.x - sclBar.x * 0.5f, screen.y - sclBar.y * 0.5 - 23);
	Vector2 defeat = Vector2(sclBar.x * 0.5f, sclBar.y * 0.5 + 23);


	// Frame
	{
		Vector2 scl = Vector2(Graphical::GetWidth(), Graphical::GetHeight()) * 0.5f;
		DrawSprite(frameTexNo, center, 0.0f, screen, Color::White);
	}

	// VICTORY
	{
		DrawSprite(victoryTexNo, victory, 0.0f, sclBar, Color::White);
		DrawSprite(defeatTexNo, defeat, 0.0f, sclBar, Color::White);
	}

	// バー
	int rank = 0;
	for (auto &client : ranking) {
		float floatY = rank - ranking.size() * 0.5f - 0.5f;
		float height = 150.0f;
		Vector2 scl = Vector2(800, 800);
		Vector2 pos = Vector2(screen.x - scl.x * 0.5f - 200, center.y - floatY * height);

		// バーの描画
		DrawSprite(barTexNo[rank], pos, 0.0f, scl, Color::White);
		// 名前の描画
		DrawSprite(nameTexNo[client.id % 4], pos, 0.0f, scl, Color::White);
		// スコアの描画
		Number(Vector2(pos.x + 270, Graphical::GetHeight() - pos.y), Vector2(100, 100), client.skillPoint);

		// 敗北者たちのアイコンの描画
		if (rank != 0) {
		Vector2 iconScl = Vector2(sclBar.y, sclBar.y);
			DrawSprite(iconTexNo[rank], Vector2(center.x + 200 + iconScl.x * (rank - 1), defeat.y), 0.0f, iconScl, Color::White);
		}
		rank++;
	}

	// キャラ
	{
		Vector2 scl = Vector2(800, 800);
		Vector2 pos = Vector2(scl.x * 0.5f, center.y + 77);
		DrawSprite(winTexNo[ranking.begin()->id % 4], pos, 0.0f, scl, Color::White);
	}

	// ログアウト
	if (game && Input::GetKeyDown(0, Input::A)) {
		game->Unregister();
	}
}

void MultiPlayLastResultModeClientSide::DrawStart(RESPONSE_PLAYER &players, Vector2 offset) {
	AllMoveScene.Move(Color::White * 0.0f);
	UIMoveScene.Move(Color::White * 0.0f);
}

void MultiPlayLastResultModeClientSide::DrawUI(RESPONSE_PLAYER &players) {

}
