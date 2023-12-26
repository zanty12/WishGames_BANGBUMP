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
		// ���݂̃����L���O�Ń\�[�g
		ranking.sort(
			[&](CLIENT_DATA_CLIENT_SIDE &a, CLIENT_DATA_CLIENT_SIDE &b) {
			if (a.skillPoint == b.skillPoint) return a.id < b.id;
			else return a.skillPoint < b.skillPoint;
			}
		);

		// 1�O�̃Q�[���̃����L���O���\�[�g
		preRanking.sort(
			[&](CLIENT_DATA_CLIENT_SIDE &a, CLIENT_DATA_CLIENT_SIDE &b) {
			if (a.skillPoint == b.skillPoint) return a.id < b.id;
			else return a.skillPoint < b.skillPoint;
			}
		);
	}

	int get_rank(std::list<CLIENT_DATA_CLIENT_SIDE> &ranking, int id) {
		int rank = 0;

		// �����N�𒲂ׂ�
		for (auto &c : ranking) {
			if (id == c.id) return rank;
			else rank++;
		}

		return -1;
	}

public:
	MultiPlayIntermediateResultModeClientSide() : MultiPlayClientSide(nullptr) {};

	void Draw(RESPONSE_PLAYER &players) override {
		const float RANKING_SORT_ANIMATION_TIME = 5.0f;				// �����L���O�̃A�j���[�V��������
		float time = players.maxTimeLimit - players.timeLimit;		// ���݂̎���



		// �����L���O�̃A�j���[�V����
		if (time <= RANKING_SORT_ANIMATION_TIME) {
			float t = time / RANKING_SORT_ANIMATION_TIME;			// ���Ԃ̊���
			auto ranking = players.clients;							// ���݂̃����L���O�\
			auto preRanking = players.clients;						// 1�Q�[���O�̃����L���O�\

			// �����L���O���\�[�g����
			sort(ranking, preRanking);


			for (auto &client : players.clients) {
				int id = client.id;									// ID
				int rank = get_rank(ranking, id);					// ���݂̃����N
				int preRank = get_rank(preRanking, id);				// 1�Q�[���O�̃����N

				DrawSprite
			}
		}
	}


	void ParseResponse(Storage &in) override {

	}
};