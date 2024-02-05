#include "multi_mode.h"
#include "multiplay.h"
#include "multi_ui.h"

void MultiPlayModeServerSide::UpdateStart(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {
}

void MultiPlayModeServerSide::UpdateResult(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {
	// 中間リザルトの経過時間を計算
	float time = time_ - maxTime_ - resultTime_;
	
	// 時間がマイナスなら終了（まだ中間リザルトではない）
	if (time < 0.0f) return;

	// はじめのみ
	if (preMode != mode && GetMode() != MULTI_MODE::CHARACTER_SELECT) {
		int maxScore = -1;
		for (int i = 0; i < clients.size(); i++) {
			int score = maxScore;
			ServerPlayer *editPlayer = nullptr;

			// 加算されていないクライアントで最大スコアの検索
			for (auto &client : clients) {
				if (client.second.player_->score <= score || score == -1) {
					score = client.second.player_->score;
					editPlayer = client.second.player_;
				}
			}

			if (editPlayer) {
				auto attribute = editPlayer->GetMoveAttribute();
				int expRange = attribute->GetLvMaxSkillOrb() - attribute->GetLvMinSkillOrb();
				editPlayer->skillPoint += expRange * (1.0f - (float)i / (float)clients.size());
			}

			score;
		}
	}
}

void MultiPlayModeClientSide::DrawStart(RESPONSE_PLAYER &players, Vector2 offset) {
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

	const float RANKING_SORT_ANIMATION_TIME = 1.0f;

	rstSkillOrb.remove_if(
		[](ResultSkillOrb skillorb) {
			return skillorb.isDestroy;
		}
	);

	// ランキングソート
	auto sortPlayers = players.clients;
	sort(sortPlayers);

	// ドロップ関数
	auto dropOrb = [&](int rank, int dropNum, float velocity) {
		for (int i = 0; i < dropNum; i++) {
			float rad = MATH::Rand(-MATH::PI, MATH::PI);
			rstSkillOrb.push_back(ResultSkillOrb(
				CalcTimePosition(),
				CalcIconPosition(rank, sortPlayers.size()),
				Vector2(std::sin(rad), std::cosf(rad) * velocity)));
		}
	};

	// 0.5秒ごとにスキルオーブをドロップ
	if (500 < dropSkillOrbCoolTimer.GetNowTime()) {
		for (auto &client : sortPlayers) {
			int id = client.id;
			int rank = get_rank(sortPlayers, id);
			int dropRate = (1.0f - (float)(rank + 1) / (float)players.clients.size());
			dropOrb(rank, 20, 30.0f);
		}
		dropSkillOrbCoolTimer.Start();
	}

	for (auto &skillOrb : rstSkillOrb) {
		skillOrb.Loop();
	}
}
