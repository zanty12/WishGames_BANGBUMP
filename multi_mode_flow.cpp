#include "multiplay.h"
#include "multi_mode_flow.h"
#include "load.h"

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
	case ENEMY_RUSH: return new MultiPlayEnemyRushModeServerSide();
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
		isNowLoad = true;
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

		isNowLoad = false;
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
	case ENEMY_RUSH: return new MultiPlayEnemyRushModeClientSide();
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


		// ゲームのスタートの画面
		if (res.time < gameMode_->startTime_) {
			gameMode_->DrawStart(res, offset);
		}
		// ゲームのリザルトの描画
		else if (0.0f < res.time - res.maxTime + gameMode_->resultTime_) {
			gameMode_->DrawResult(res, offset);
		}
		// ゲームモードの描画
		else {
			gameMode_->Draw(res, offset);
		}


		float centerX = Graphical::GetWidth() * 0.5f;		// 画面の中央（X座標）

		// 時間制限の描画
		DrawSpriteLeftTop(timerTexNo,
			Vector2(centerX, 100), 0.0f, Vector2(1000, 250) * 0.5f,
			Color::White,
			Vector2::Zero, Vector2::One
		);
		Number(Vector2(centerX, 100), Vector2(100, 100), res.maxTime - res.time);

		// スコアの描画
		int idx = 0;										// インデックス
		int maxMembers = res.clients.size();				// プレイヤー人数
		int width = 300;									// アイコンの幅
		for (auto &client : res.clients) {
			int moveAttribute = client.moveAttributeType;
			int attackAttribute = client.attackAttributeType;
			//画像の関係上Attackをずらす
			if (moveAttribute < attackAttribute) attackAttribute--;
			float u = moveAttribute / 4.0f;
			float v = attackAttribute / 12.0f;
			Vector2 uv = Vector2(u, v + idx * 0.25f);
			Vector2 uvScale = Vector2(0.25f, 1.0f / 12.0f);

			float center = (float)maxMembers * 0.5f - 0.5f;	// 中心のIdxを計算
			float x = center - idx;							// X座標を計算
			x *= -1;

			uv = Vector2::Zero;
			uvScale = Vector2::One;
			DrawSprite(icon3,
				Vector2(centerX + x * width, 100), 0.0f, Vector2(200, 100),
				Color::White,
				uv, uvScale
			);
			DrawSprite(icon2,
				Vector2(centerX + x * width, 100), 0.0f, Vector2(200, 100),
				Color::White,
				uv, uvScale
			);
			DrawSprite(icon,
				Vector2(centerX + x * width, 100), 0.0f, Vector2(200, 100),
				Color::White,
				uv, uvScale
			);

			Number(Vector2(centerX + x * width, 200), Vector2(100, 100), client.skillPoint);
			idx++;
		}
	}
}

void MultiPlayFlowClientSide::ParseResponse(Storage &in) {
	if (gameMode_) {
		gameMode_->ParseResponse(in);
	}
}
