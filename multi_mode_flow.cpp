#include "multiplay.h"
#include "multi_mode_flow.h"

/***********************************************************
	Server
************************************************************/
MultiPlayServerSide *MultiPlayFlowServerSide::CreateMode(MULTI_MODE mode) {
	startTime = timeGetTime();
	switch (mode)
	{
	case CHARACTER_SELECT: return new MultiPlayCharacterSelectModeServerSide(game_);
	case AREA_CAPTURE: return new MultiPlayAreaCaptureModeServerSide(game_);
	case INTERMEDIATE_RESULT_1: return new MultiPlayIntermediateResult1ModeServerSide();
	case OBSTACLE_RACE: return new MultiPlayObstacleRaceModeServerSide(game_);
	case INTERMEDIATE_RESULT_2: return new MultiPlayIntermediateResult2ModeServerSide();
	case ENEMY_RUSH: return new MultiPlayEnemyRushModeServerSide(game_);
	case INTERMEDIATE_RESULT_3: return new MultiPlayIntermediateResult3ModeServerSide();
	case FINAL_BATTLE: return nullptr;
	}
	return nullptr;
}

void MultiPlayFlowServerSide::Update(std::list<CLIENT_DATA_SERVER_SIDE> &clients) {
	// ゲームモードがないなら終了
	if (gameMode_ == nullptr) return;

	// 制限時間が来たなら、次のモードへ移行
	if (gameMode_->maxTime_ < gameMode_->time_) {
		// 現在のモードの取得
		MULTI_MODE mode_ = GetMode();

		// 現在のモードの削除
		gameMode_->Release(clients);
		delete gameMode_;
		gameMode_ = nullptr;

		// 次のモードを計算
		mode_ = (MULTI_MODE)((int)mode_ + 1);

		// 次のモードの作成
		gameMode_ = CreateMode(mode_);
	}
	else {
		float deltaTime = (timeGetTime() - startTime) * 0.001f;
		gameMode_->time_ = (timeGetTime() - startTime) * 0.001f;

		gameMode_->Update(clients);
	}
}

void MultiPlayFlowServerSide::CreateResponse(Storage &out) {
	if (gameMode_) {
		gameMode_->CreateResponse(out);
	}
}





/***********************************************************
	Client
************************************************************/
MultiPlayClientSide *MultiPlayFlowClientSide::CreateMode(MULTI_MODE mode) {
	switch (mode)
	{
	case CHARACTER_SELECT: return new MultiPlayCharacterSelectModeClientSide(game_);
	case AREA_CAPTURE: return new MultiPlayAreaCaptureModeClientSide(game_);
	case INTERMEDIATE_RESULT_1: return new MultiPlayIntermediateResult1ModeClientSide();
	case OBSTACLE_RACE: return new MultiPlayObstacleRaceModeClientSide(game_);
	case INTERMEDIATE_RESULT_2: return new MultiPlayIntermediateResult2ModeClientSide();
	case ENEMY_RUSH: return new MultiPlayEnemyRushModeClientSide(game_);
	case INTERMEDIATE_RESULT_3: return new MultiPlayIntermediateResult3ModeClientSide();
	case FINAL_BATTLE: return nullptr;
	}
	return nullptr;
}

void MultiPlayFlowClientSide::Draw(RESPONSE_PLAYER &res) {
	std::cout << res.mode << std::endl;

	// モードが切り替わったなら、次のモードへ移行
	if (currentMode_ != res.mode) {
		// 現在のモードの取得
		MULTI_MODE mode_ = GetMode();

		// 現在のモードの削除
		if (gameMode_) delete gameMode_;

		// 次のモードの作成
		gameMode_ = CreateMode(res.mode);

		// 次のモードの取得
		currentMode_ = GetMode();
	}
	// モードの実行
	else if (gameMode_) {
		gameMode_->Draw(res);
		// 時間制限の描画
		Number(Vector2(100, 100), Vector2(100, 100), res.maxTime - res.time);

		// スコアの描画
		int i = 0;
		for (auto &client : res.clients) {
			int moveAttribute = client.moveAttributeType;
			int attackAttribute = client.attackAttributeType;
			//画像の関係上Attackをずらす
			if (moveAttribute < attackAttribute) attackAttribute--;
			float u = moveAttribute / 4.0f;
			float v = attackAttribute / 12.0f;
			Vector2 uv = Vector2(u, v + i * 0.25f);

			DrawSprite(icon,
				Vector2(100 + i * 100, 100), 0.0f, Vector2(100, 100),
				Color::White,
				uv, Vector2(0.25f, 1.0f / 12.0f)
				);
			Number(Vector2(200, 200), Vector2(100, 100), client.skillPoint + 1);
			i++;
		}
	}
}

void MultiPlayFlowClientSide::ParseResponse(Storage &in) {
	if (gameMode_) {
		gameMode_->ParseResponse(in);
	}
}
