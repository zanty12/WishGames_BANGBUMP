#include "multi_mode.h"
#include "multiplay.h"
#include "multi_ui.h"
#include "move_scene_anim.h"

void MultiPlayModeServerSide::UpdateStart(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {
	// 開始の経過時間を計算
	float time = time_;

	isPlayerMove = 5.0f < time;

	
}

void MultiPlayModeServerSide::UpdateResult(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {
	// 中間リザルトの経過時間を計算
	float time = time_ - maxTime_ + resultTime_;
	
	// 時間がマイナスなら終了（まだ中間リザルトではない）
	if (time < 0.0f) return;

	// 移動できないようにする
	isPlayerMove = false;

	// はじめのみ
	if (preMode != mode && GetMode() != MULTI_MODE::CHARACTER_SELECT) {
		// ランキング
		std::list<ServerPlayer *> ranking;
		for (auto &kvp : MultiPlayServer::clients_) ranking.push_back(kvp.second.player_);

		// ソート
		ranking.sort([](ServerPlayer *a, ServerPlayer *b) {
			return a->score > b->score;
			}
		);

		int addRank = 1;
		int rank = -1;
		int prevScore = -1;
		for (auto &player : ranking) {
			if (player->score == prevScore) {
				addRank++;
			}
			else {
				rank += addRank;
				addRank = 1;
			}
			prevScore = player->score;


			int expRange = player->GetLvMaxSkillOrb() - player->GetLvMinSkillOrb();
			switch (rank) {
			case 0: player->skillPoint += expRange * 0.7f; break;
			case 1: player->skillPoint += expRange * 0.5f; break;
			case 2: player->skillPoint += expRange * 0.2f; break;
			case 3: player->skillPoint += expRange * 0.0f; break;
			}

			rank += addRank;
		}
	}
}

void MultiPlayModeServerSide::Release(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {
	// プレイヤーの状態の初期化
	for (auto kvp : MultiPlayServer::clients_) {
		auto player = kvp.second.player_;
		player->animType = ANIMATION_TYPE_IDLE;
		player->attackVelocity = player->chargeVelocity = player->blownVelocity = player->velocity = Vector2::Zero;
	}
}

void MultiPlayModeClientSide::DrawUI(RESPONSE_PLAYER &players) 
{
	// カウントダウン（終了時）
	{
		float countDown = players.maxTime - resultTime_ - players.time;
		// カウントダウン
		if (0.0f <= countDown && countDown < 4.0f) {
			float centerX = Graphical::GetWidth() * 0.5f;
			float centerY = Graphical::GetHeight() * 0.5f;

			float vh = 1.0f / 4.0f;
			float v = (int)countDown * vh;
			float t = countDown - (int)countDown;
			float rate = MATH::Leap(0.4f, 1.0f, t * t);


			if (1.0f < countDown) {
				DrawSprite(countDownTexNo, Vector2(centerX, centerY), 0.0f, Vector2(800, 800) * rate,
					Color(1, 1, 1, rate), Vector2(0.0f, v), Vector2(1.0, vh));
				timeupAnim.MoveBegin();
			}
			// タイムアップ
			else {
				timeupAnim.Draw(Vector2(centerX, centerY), 0.0f, Vector2(800, 800), Color::White);
			}
		}
	}

	// カウントダウン（開始時）
	{
		float time = players.time;

		// カウントダウンが無いなら説明を表示
		float countDown = startTime_ - time;
		float centerX = Graphical::GetWidth() * 0.5f;
		float centerY = Graphical::GetHeight() * 0.5f;

		if (0.0f <= countDown && countDown < 4.0f) {
			float vh = 1.0f / 4.0f;
			float v = (int)countDown * vh;
			float t = countDown - (int)countDown;
			float rate = MATH::Leap(0.4f, 1.0f, t * t);

			DrawSprite(countDownTexNo, Vector2(centerX, centerY), 0.0f, Vector2(800, 800) * rate,
				Color(1, 1, 1, rate), Vector2(0.0f, v), Vector2(1.0, vh));
		}
	}

	// 情報
	{
		float time = players.time;

		// カウントダウンが無いなら説明を表示
		float countDown = startTime_ - time;
		float centerX = Graphical::GetWidth() * 0.5f;
		float centerY = Graphical::GetHeight() * 0.5f;
		if (4.0f <= countDown) {
			DrawSprite(descTexNo, Vector2(centerX, centerY), 0.0f, Vector2(1000, 527),
				Color(1, 1, 1), Vector2::Zero, Vector2::One);
		}
	}

	// ステージ名の描画
	{
		float time = players.time;
		const float STAGE_NAME_ANIMATION_START_TIME = 5.0f;		// ステージ名表示開始
		const float STAGE_NAME_ANIMATION_TIME = 4.0f;			// ステージ名表示中
		// ステージ名
		if (STAGE_NAME_ANIMATION_START_TIME <= time && time <= STAGE_NAME_ANIMATION_START_TIME + STAGE_NAME_ANIMATION_TIME) {
			float t = (time - STAGE_NAME_ANIMATION_START_TIME) / STAGE_NAME_ANIMATION_TIME;

			if (t <= 1.0f) {
				float centerX = Graphical::GetWidth() * 0.5f;
				float centerY = Graphical::GetHeight() * 0.5f;


				float y = MATH::Bezier(centerY + 150.0f, centerY + 50.0f, centerY - 25.0f, centerY - 50.0f, t);
				float alpha = MATH::Bezier(0.0f, 2.0f, 1.5f, 0.0f, t);

				DrawSprite(stageNameTexNo, Vector2(centerX, y), 0.0f, Vector2(1200.0f, 500.0f) * 0.75f, Color::White * alpha);
			}
		}
	}
}

void MultiPlayModeClientSide::DrawStart(RESPONSE_PLAYER &players, Vector2 offset) {
	float time = players.time;


	const float SPAWN_ANIMATION_START_TIME = 2.0f;			// 登場表示開始
	const float SPAWN_ANIMATION_TIME = 3.0f;				// 登場中




	float spawnSpanTime = SPAWN_ANIMATION_TIME / players.clients.size();	// スポーンさせる間隔

	// カウントダウンが無いなら説明を表示
	float countDown = startTime_ - time;
	float centerX = Graphical::GetWidth() * 0.5f;
	float centerY = Graphical::GetHeight() * 0.5f;
	//if (4.0f <= countDown) {
	//	DrawSprite(descTexNo, Vector2(centerX, centerY), 0.0f, Vector2(1000, 527),
	//		Color(1, 1, 1), Vector2::Zero, Vector2::One);
	//}

	// スポーンさせる
	if (AllMoveScene.Move(Color::White * 0.0f) && UIMoveScene.Move(Color::White * 0.0f) && SPAWN_ANIMATION_START_TIME + spawnSpanTime * clientSpawnCount <= time) {

		// イテレータ
		auto iterator = players.clients.begin();

		// アニメーションするプレイヤーのイテレータ
		for (int i = 0; i < clientSpawnCount && iterator != players.clients.end(); i++) iterator++;

		// イテレータが存在するなら
		if (iterator != players.clients.end()) {
			// アニメーションする
			MultiPlayClient::clients[iterator->id]->ShowEntry();
		}
		clientSpawnCount++;
	}




}

void MultiPlayModeClientSide::DrawResult(RESPONSE_PLAYER &players, Vector2 offset) {
	// 中間リザルトの経過時間を計算
	float time = players.time - players.maxTime + resultTime_;

	// 時間がマイナスなら終了（まだ中間リザルトではない）
	if (time < 0.0f) return;

	const float FADE_ANIMATION = 0.5f;
	const float RANK_ANIMATION = 3.5f;
	const float DROP_ANIMATION = 5.5f;
	const float STAY_ANIMATION = 7.5f;
	const float WARP_ANIMATION = 11.5f;



	rstSkillOrb.remove_if(
		[](ResultSkillOrb skillorb) {
			return skillorb.isDestroy;
		}
	);
	if (time <= FADE_ANIMATION) {
		UIMoveScene.Move(Color::Black * 0.75);
		clientSpawnCount = 0;
	}

	// ランキングアニメーション
	else if (time <= RANK_ANIMATION) {
		// ランキング
		std::list<ClientPlayer *> ranking;
		for (auto &kvp : MultiPlayClient::clients) ranking.push_back(kvp.second);

		// ソート
		ranking.sort([](ClientPlayer *a, ClientPlayer *b) {
			return a->score > b->score;
			}
		);

		const float RANK_TIME = (time - FADE_ANIMATION);
		const float ONCE_RANK_TIME = 0.5f;

		// アニメーション
		int addRank = 1;
		int rank = -1;
		int prevScore = -1;
		for (auto &player : ranking) {
			if (player->score == prevScore) {
				addRank++;
			}
			else {
				rank += addRank;
				addRank = 1;
			}
			prevScore = player->score;


			if (ONCE_RANK_TIME * rank < time) {

				// アイコンの座標を取得
				Vector2 position = CalcIconPosition(player->id % 4, ranking.size());
				Vector2 scale = Vector2(600.0f, 400.0f);
				switch (rank) {
				case 0: scale *= 0.5f; break;
				case 1: scale *= 0.125f; break;
				case 2: scale *= 0.09375f; break;
				case 3: scale *= 0.0625f; break;
				}


				Vector2 pos1 = Vector2(0.0f, 0.0f);
				Vector2 pos2 = Vector2(0.0f, 20.0f);
				Vector2 pos3 = Vector2(0.0f, 80.0f);
				Vector2 pos4 = Vector2(0.0f, 100.0f);
				Color col1 = Color(1.0f, 1.0f, 1.0f, 0.0f);
				Color col2 = Color(1.0f, 1.0f, 1.0f, 0.2f);
				Color col3 = Color(1.0f, 1.0f, 1.0f, 0.8f);
				Color col4 = Color(1.0f, 1.0f, 1.0f, 1.0f);
				float addRate = 0.02f;
				rankAnim[rank].Draw(position, scale, pos1, pos2, pos3, pos4, col1, col2, col3, col4, addRate);
			}
		}
	}

	// ドロップアニメーション
	else if (time <= DROP_ANIMATION) {
		// ランキング
		std::list<ClientPlayer *> ranking;
		for (auto &kvp : MultiPlayClient::clients) ranking.push_back(kvp.second);

		// ソート
		ranking.sort([](ClientPlayer *a, ClientPlayer *b) {
			return a->score > b->score;
			}
		);

		// ドロップ関数
		auto dropOrb = [&](int idx, int dropNum, float velocity) {
			for (int i = 0; i < dropNum; i++) {
				float rad = MATH::Rand(-MATH::PI, MATH::PI);
				rstSkillOrb.push_back(ResultSkillOrb(
					CalcTimePosition(),
					CalcIconPosition(idx, ranking.size()),
					Vector2(std::sin(rad), std::cosf(rad) * velocity)));
			}
		};

		// 0.5秒ごとにスキルオーブをドロップ
		if (100 < dropSkillOrbCoolTimer.GetNowTime()) {
			int addRank = 1;
			int rank = -1;
			int prevScore = -1;
			for (auto &player : ranking) {
				if (player->score == prevScore) {
					addRank++;
				}
				else {
					rank += addRank;
					addRank = 1;
				}
				prevScore = player->score;
				int dropNum = 5;

				int expRange = player->GetLvMaxSkillOrb() - player->GetLvMinSkillOrb();
				switch (rank) {
				case 0: dropOrb(player->id, dropNum * 1.5f, 30.0f); break;
				case 1: dropOrb(player->id, dropNum * 0.5f, 30.0f); break;
				case 2: dropOrb(player->id, dropNum * 0.25f, 30.0f); break;
				case 3: dropOrb(player->id, dropNum * 0.0f, 30.0f); break;
				}
			}
			dropSkillOrbCoolTimer.Start();
		}
	}
	else if (time <= STAY_ANIMATION) {
	}
	else if (time <= WARP_ANIMATION) {
		float time = players.time;


		const float SPAWN_ANIMATION_TIME = 3.0f;
		float spawnSpanTime = SPAWN_ANIMATION_TIME / players.clients.size();	// スポーンさせる間隔

		// スポーンさせる
		if (spawnSpanTime * clientSpawnCount <= time) {

			// イテレータ
			auto iterator = players.clients.begin();

			// アニメーションするプレイヤーのイテレータ
			for (int i = 0; i < clientSpawnCount && iterator != players.clients.end(); i++) iterator++;

			// イテレータが存在するなら
			if (iterator != players.clients.end()) {
				// アニメーションする
				MultiPlayClient::clients[iterator->id]->ShowExit();
			}
			clientSpawnCount++;
		}
	}
	else {
		AllMoveScene.Move(Color::White * 1.0f);
	}

	for (auto &skillOrb : rstSkillOrb) {
		skillOrb.Loop();
	}
}
