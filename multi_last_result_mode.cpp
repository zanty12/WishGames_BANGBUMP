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


	// Frame
	{
		Vector2 scl = Vector2(Graphical::GetWidth(), Graphical::GetHeight()) * 0.5f;
		DrawSprite(frameTexNo, center, 0.0f, screen, Color::White);
		std::cout << "FFRAMW : " << frameTexNo << std::endl;
	}

	// VICTORY
	{
		Vector2 scl = Vector2(1899, 324) * 0.5f;
		DrawSprite(victoryTexNo, Vector2(screen.x - scl.x * 0.5f, screen.y - scl.y * 0.5 - 15), 0.0f, scl, Color::White);
		DrawSprite(defeatTexNo, Vector2(scl.x * 0.5f, scl.y * 0.5 + 15), 0.0f, scl, Color::White);
	}

	// ƒo[
	int id = 0;
	for (auto &client : ranking) {
		float floatY = ranking.size() * 0.5f - 0.5f;
		float height = 50.0f;
		Vector2 scl = Vector2(800, 800);
		Vector2 pos = Vector2(screen.x - scl.x * 0.5f - 200, center.y - floatY * height);

		DrawSprite(barTexNo[id], pos, 0.0f, scl, Color::White);
		DrawSprite(nameTexNo[client.id % 4], pos, 0.0f, scl, Color::White);
		//DrawSprite(iconTexNo[i], pos, 0.0f, scl, Color::White);
		id++;
	}

	// ƒLƒƒƒ‰
	{
		Vector2 scl = Vector2(800, 800);
		Vector2 pos = Vector2(scl.x * 0.5f, center.y + 77);
		DrawSprite(winTexNo[ranking.begin()->id % 4], pos, 0.0f, scl, Color::White);
	}
}
