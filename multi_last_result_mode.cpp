#include "multi_last_result_mode.h"

void MultiPlayLastResultModeClientSide::Draw(RESPONSE_PLAYER &players, Vector2 offset) {
	if (ranking.size() == 0) {
		ranking = players.clients;
		ranking.sort([](CLIENT_DATA_CLIENT_SIDE &a, CLIENT_DATA_CLIENT_SIDE &b) {
			return a.skillPoint < b.skillPoint;
			}
		);
	}



	Vector2 screen = Vector2(Graphical::GetWidth(), Graphical::GetHeight());
	Vector2 center = screen * 0.5f;
	int id = 0;


	// VICTORY
	{
		Vector2 scl = Vector2(1899, 324) * 0.75f;
		DrawSprite(victoryTexNo, Vector2(screen.x - scl.x * 0.5f, screen.y * Graphical::GetHeight() - scl.y * 0.5), 0.0f, scl, Color::White);
		DrawSprite(victoryTexNo, center, 0.0f, scl, Color::White);
	}

	// ƒo[
	for (int i = 0; i < ranking.size(); i++) {
		float floatY = ranking.size() * 0.5f - 0.5f;
		float height = 50.0f;
		Vector2 pos = Vector2(1500, floatY * height);
		Vector2 scl = Vector2(200, 200);

		DrawSprite(barTexNo[i], Vector2(screen.x + scl.x * 0.5f, screen.y * Graphical::GetHeight() - scl.y * 0.5), 0.0f, scl, Color::White);
		DrawSprite(iconTexNo[i], Vector2(screen.x + scl.x * 0.5f, screen.y * Graphical::GetHeight() - scl.y * 0.5), 0.0f, scl, Color::White);
		DrawSprite(winTexNo[i], Vector2(screen.x + scl.x * 0.5f, screen.y * Graphical::GetHeight() - scl.y * 0.5), 0.0f, scl, Color::White);

	}
}
