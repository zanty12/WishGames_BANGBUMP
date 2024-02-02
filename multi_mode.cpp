#include "multi_mode.h"

void MultiPlayModeServerSide::UpdateResult(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {
	// 中間リザルトの経過時間を計算
	float time = time_ - maxTime_ - resultTime_;
	
	// 時間がマイナスなら終了（まだ中間リザルトではない）
	if (time < 0.0f) return;


}

void MultiPlayModeClientSide::DrawResult(RESPONSE_PLAYER &players, Vector2 offset) {
	// 中間リザルトの経過時間を計算
	float time = players.time - players.maxTime + resultTime_;

	// 時間がマイナスなら終了（まだ中間リザルトではない）
	if (time < 0.0f) return;

	auto afterClients = players.clients;
	const float RANKING_SORT_ANIMATION_TIME = 1.0f;

	// ランキングのアニメーション
	if (time <= RANKING_SORT_ANIMATION_TIME) {

		// ランキングをソートする
		sort(beforeClients);									// ゲーム開始時のランキング表
		sort(afterClients);										// 現在のランキング表

		int playerNum = players.clients.size();					// プレイヤー数
		float t = time / RANKING_SORT_ANIMATION_TIME;			// 時間の割合
		t *= t;													// 二次関数に変換させる


		for (auto &client : players.clients) {
			int id = client.id;									// ID
			int preRank = get_rank(beforeClients, id);			// 1ゲーム前のランク
			int rank = get_rank(afterClients, id);				// 現在のランク

			float height = 100.0f;								// キャラクター絵の高さ
			float y = rank - playerNum * 0.5f;					// 現在のランキングのY座標
			float preY = preRank - playerNum * 0.5f;			// 1ゲーム前のランキングのY座標
			Vector2 startPosition = Vector2::Up * y * height;	// 始点の座標
			Vector2 endPosition = Vector2::Up * preY * height;	// 終点の座標
			Vector2 position = MATH::Leap(startPosition, endPosition, t);	// 線形補完
			DrawSpriteCenter(0, position, 0.0f, Vector2(600, height), Color::White);
		}
	}
}
