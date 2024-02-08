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


		int rank = 0;
		int preScore = -1;
		int addRank = 1;
		for (auto player : ranking) {
			if (player->score == preScore) addRank++;
			else addRank = 1;

			int expRange = player->GetLvMaxSkillOrb() - player->GetLvMinSkillOrb();
			switch (rank) {
			case 0: player->skillPoint += expRange * 0.7f; break;
			case 1: player->skillPoint += expRange * 0.5f; break;
			case 2: player->skillPoint += expRange * 0.2f; break;
			case 3: player->skillPoint += expRange * 0.0f; break;
			}

			rank += addRank;
			preScore = player->score;
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

void MultiPlayModeClientSide::DrawStart(RESPONSE_PLAYER &players, Vector2 offset) {
	float time = players.time;


	const float SPAWN_ANIMATION_START_TIME = 2.0f;			// 登場表示開始
	const float STAGE_NAME_ANIMATION_START_TIME = 5.0f;		// ステージ名表示開始

	const float SPAWN_ANIMATION_TIME = 3.0f;				// 登場中
	const float STAGE_NAME_ANIMATION_TIME = 2.0f;			// ステージ名表示中




	float spawnSpanTime = SPAWN_ANIMATION_TIME / players.clients.size();	// スポーンさせる間隔

	// スポーンさせる
	if (MoveScene::Move(Color::White * 0.0f) && SPAWN_ANIMATION_START_TIME + spawnSpanTime * clientSpawnCount <= time) {

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

	// ステージ名
	else if (STAGE_NAME_ANIMATION_START_TIME <= time) {
		float t = (time - STAGE_NAME_ANIMATION_START_TIME) / STAGE_NAME_ANIMATION_TIME;
		float centerX = Graphical::GetWidth() * 0.5f;
		float centerY = Graphical::GetHeight() * 0.5f;


		float y = MATH::Bezier(y + 200.0f, y + 50.0f, y - 50.0f, y - 100.0f, t);
		float a = MATH::Bezier(0.0f, 1.0f, 0.5f, 0.0f, t);
		
	}
}

void MultiPlayModeClientSide::DrawResult(RESPONSE_PLAYER &players, Vector2 offset) {
	// 中間リザルトの経過時間を計算
	float time = players.time - players.maxTime + resultTime_;

	// 時間がマイナスなら終了（まだ中間リザルトではない）
	if (time < 0.0f) return;

	const float FADE_ANIMATION = 0.5f;
	const float DROP_ANIMATION = 2.5f;
	const float STAY_ANIMATION = 3.5f;
	const float WARP_ANIMATION = 9.5f;



	rstSkillOrb.remove_if(
		[](ResultSkillOrb skillorb) {
			return skillorb.isDestroy;
		}
	);
	if (time <= FADE_ANIMATION) {
		MoveScene::Move(Color::Black * 0.5f);
		clientSpawnCount = 0;
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
			int rank = 0;
			int preScore = -1;
			int addRank = 1;
			int dropNum = 5;
			for (auto player : ranking) {
				if (player->score == preScore) addRank++;
				else addRank = 1;

				int expRange = player->GetLvMaxSkillOrb() - player->GetLvMinSkillOrb();
				switch (rank) {
				case 0: dropOrb(player->id, dropNum * 1.5f, 30.0f); break;
				case 1: dropOrb(player->id, dropNum * 0.5f, 30.0f); break;
				case 2: dropOrb(player->id, dropNum * 0.25f, 30.0f); break;
				case 3: dropOrb(player->id, dropNum * 0.0f, 30.0f); break;
				}
				rank += addRank;
				preScore = player->score;
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
		MoveScene::Move(Color::White * 1.0f);
	}

	for (auto &skillOrb : rstSkillOrb) {
		skillOrb.Loop();
	}
}
