#include "multiplay.h"
#include "multi_mode_flow.h"

/***********************************************************
	Server
************************************************************/
MultiPlayModeServerSide *MultiPlayFlowServerSide::CreateMode(MULTI_MODE mode) {
	startTime = timeGetTime();
	MultiPlayModeServerSide *rstMode = nullptr;
	switch (mode)
	{
	case CHARACTER_SELECT: rstMode = new MultiPlayCharacterSelectModeServerSide(game_); break;
	case AREA_CAPTURE: rstMode = new MultiPlayAreaCaptureModeServerSide(); break;
	//case INTERMEDIATE_RESULT_1: return new MultiPlayIntermediateResult1ModeServerSide();
	//case OBSTACLE_RACE: return new MultiPlayObstacleRaceModeServerSide(game_);
	//case INTERMEDIATE_RESULT_2: return new MultiPlayIntermediateResult2ModeServerSide();
	//case ENEMY_RUSH: return new MultiPlayEnemyRushModeServerSide(game_);
	//case INTERMEDIATE_RESULT_3: return new MultiPlayIntermediateResult3ModeServerSide();
	case FINAL_BATTLE: rstMode = new MultiPlayFinalBattleModeServerSide(); break;
	}

	// 現在いるマップの更新
	if (rstMode) {
		for (auto &client : MultiPlayServer::clients_) client.second.player_->map = rstMode->map_;
	}

	return rstMode;
}

void MultiPlayFlowServerSide::Update(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {
	// ゲームモードがないなら終了
	if (gameMode_ == nullptr) return;

	// 制限時間が来たなら、次のモードへ移行
	if (gameMode_->maxTime_ < gameMode_->time_ || gameMode_->isSkip) {
		// 現在のモードの取得
		MULTI_MODE mode_ = GetMode();

		// 現在のモードの削除
		if (gameMode_) {
			// 現在のモードのリリース関数を呼び出す
			gameMode_->Release(clients);

			delete gameMode_;
		}

		// 次のモードを計算
		mode_ = (MULTI_MODE)((int)mode_ + 1);

		// 次のモードの作成
		gameMode_ = CreateMode(mode_);

		// プレイヤーの移動
		if (gameMode_ && gameMode_->map_->startPosition.size()) {
			auto spawnPosIterator = gameMode_->map_->startPosition.begin();
			for (auto &client : clients) {
				// 移動する
				auto &player = client.second.player_;
				player->transform.position = *spawnPosIterator;

				// スタート地点を1つずらす
				spawnPosIterator++;

				// もしスタート地点がないなら、リストの最初からにする
				if (gameMode_->map_->startPosition.end() == spawnPosIterator) {
					spawnPosIterator = gameMode_->map_->startPosition.begin();
				}
			}
		}
	}
	else {
		// 時間の更新
		float deltaTime = (timeGetTime() - startTime) * 0.001f;
		gameMode_->time_ = deltaTime;



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
MultiPlayModeClientSide *MultiPlayFlowClientSide::CreateMode(MULTI_MODE mode) {
	switch (mode)
	{
	case CHARACTER_SELECT: return new MultiPlayCharacterSelectModeClientSide(game_);
	case AREA_CAPTURE: return new MultiPlayAreaCaptureModeClientSide();
	//case INTERMEDIATE_RESULT_1: return new MultiPlayIntermediateResult1ModeClientSide();
	//case OBSTACLE_RACE: return new MultiPlayObstacleRaceModeClientSide(game_);
	//case INTERMEDIATE_RESULT_2: return new MultiPlayIntermediateResult2ModeClientSide();
	//case ENEMY_RUSH: return new MultiPlayEnemyRushModeClientSide(game_);
	//case INTERMEDIATE_RESULT_3: return new MultiPlayIntermediateResult3ModeClientSide();
	case FINAL_BATTLE: return new MultiPlayFinalBattleModeClientSide();
	}
	return nullptr;
}

void MultiPlayFlowClientSide::Draw(RESPONSE_PLAYER &res, Vector2 offset) {

	// モードが切り替わったなら、次のモードへ移行
	if (currentMode_ != res.mode) {
		// 現在のモードの取得
		MULTI_MODE mode_ = GetMode();

		// 現在のモードの削除
		if (gameMode_) {
			// 現在のモードのリリース関数を呼び出す
			gameMode_->Release(res);
			
			delete gameMode_;
		}

		// 次のモードの作成
		gameMode_ = CreateMode(res.mode);

		// 次のモードの取得
		currentMode_ = GetMode();
	}
	// モードの実行
	else if (gameMode_) {

		// マップの描画
		gameMode_->map_->Draw(offset);



		// ゲームのリザルトの描画
		std::cout << res.time << " - " << gameMode_->resultTime_ << " = " << res.time - gameMode_->resultTime_ << std::endl;
		if (0.0f < res.time - gameMode_->resultTime_) {
			gameMode_->DrawResult(res, offset);
		}
		// ゲームモードの描画
		else {
			gameMode_->Draw(res, offset);
		}


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
			Number(Vector2(200 * (i + 1), 200), Vector2(100, 100), client.skillPoint + 1);
			i++;
		}
	}
}

void MultiPlayFlowClientSide::ParseResponse(Storage &in) {
	if (gameMode_) {
		gameMode_->ParseResponse(in);
	}
}
