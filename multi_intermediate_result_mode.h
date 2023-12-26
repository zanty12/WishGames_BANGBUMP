#pragma once
#include "multi_mode.h"
#include "lib/math.h"

class MultiPlayIntermediateResultModeServerSide : public MultiPlayServerSide {
protected:
	void GameUpdate(std::list<CLIENT_DATA_SERVER_SIDE> &clients) override {

	}

public:
	MultiPlayIntermediateResultModeServerSide() : MultiPlayServerSide(nullptr) {
		timeLimit_ = 20.0f;
	}
};

class MultiPlayIntermediateResultModeClientSide : public MultiPlayClientSide {
private:
	void sort(std::list<CLIENT_DATA_CLIENT_SIDE>& ranking, std::list<CLIENT_DATA_CLIENT_SIDE> &preRanking) {
		// 現在のランキングでソート
		ranking.sort(
			[&](CLIENT_DATA_CLIENT_SIDE &a, CLIENT_DATA_CLIENT_SIDE &b) {
			if (a.skillPoint == b.skillPoint) return a.id < b.id;
			else return a.skillPoint < b.skillPoint;
			}
		);

		// 1つ前のゲームのランキングをソート
		preRanking.sort(
			[&](CLIENT_DATA_CLIENT_SIDE &a, CLIENT_DATA_CLIENT_SIDE &b) {
			if (a.skillPoint == b.skillPoint) return a.id < b.id;
			else return a.skillPoint < b.skillPoint;
			}
		);
	}

	int get_rank(std::list<CLIENT_DATA_CLIENT_SIDE> &ranking, int id) {
		int rank = 0;

		// ランクを調べる
		for (auto &c : ranking) {
			if (id == c.id) return rank;
			else rank++;
		}

		return -1;
	}

public:
	MultiPlayIntermediateResultModeClientSide() : MultiPlayClientSide(nullptr) {};

	void Draw(RESPONSE_PLAYER &players) override {
		const float RANKING_SORT_ANIMATION_TIME = 5.0f;				// ランキングのアニメーション時間
		float time = players.maxTimeLimit - players.timeLimit;		// 現在の時間



		// ランキングのアニメーション
		if (time <= RANKING_SORT_ANIMATION_TIME) {
			float t = time / RANKING_SORT_ANIMATION_TIME;			// 時間の割合
			auto ranking = players.clients;							// 現在のランキング表
			auto preRanking = players.clients;						// 1ゲーム前のランキング表

			// ランキングをソートする
			sort(ranking, preRanking);


			for (auto &client : players.clients) {
				int id = client.id;									// ID
				int rank = get_rank(ranking, id);					// 現在のランク
				int preRank = get_rank(preRanking, id);				// 1ゲーム前のランク

				DrawSprite
			}
		}
	}


	void ParseResponse(Storage &in) override {

	}
};