#pragma once
#include "multi_header.h"
#include "multi_runenum.h"
#include "multi_map.h"
#include "multi_result_skillorb.h"
#include "time.h"

class MultiPlayFlowServerSide;
class MultiPlayModeServerSide {
protected:
	enum MODE {
		START,
		PLAY,
		RESULT,
	};
	float maxTime_ = 60.0f;
	float time_ = 0.0f;
	float startTime_ = 15.0f;			// 開始までの時間
	float resultTime_ = 15.0f;			// 中間リザルトの時間
	bool isSkip = false;
	MultiMap *map_ = nullptr;
	friend MultiPlayFlowServerSide;


public:
	int mode = NONE;
	int preMode = NONE;
	MultiPlayModeServerSide(MultiMap *map, std::wstring modeName) : map_(map) {
		startTime_ = ini::GetFloat(PARAM_PATH + L"mode.ini", modeName.c_str(), L"startTime");
		resultTime_ = ini::GetFloat(PARAM_PATH + L"mode.ini", modeName.c_str(), L"resultTime");
		maxTime_ = ini::GetFloat(PARAM_PATH + L"mode.ini", modeName.c_str(), L"timeLimit") + startTime_ + resultTime_;

	}
	~MultiPlayModeServerSide() { if (map_) delete map_; }
	virtual void Update(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) { };
	virtual void UpdateStart(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients);
	virtual void UpdateResult(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients);
	virtual void CreateResponse(Storage& out) { };
	virtual void Release(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) { };

	virtual bool IsPlayerMove(void) const { return time_ <= maxTime_; };
	virtual MULTI_MODE GetMode(void) const = 0;
	float GetTime(void) const { return time_; }
	float GetMaxTime(void) const { return maxTime_; }
	MultiMap *GetMap(void) { return map_; }
};




class MultiPlayFlowClientSide;
class MultiPlayModeClientSide {
protected:
	float startTime_ = 15.0f;							// 開始までの時間
	float resultTime_ = 15.0f;							// 中間リザルトの時間
	MultiMap *map_;
	int clientSpawnCount = 0;							// クライアントのスポーンカウント
	std::list<ResultSkillOrb> rstSkillOrb;				// リザルト時のスキルオーブ
	WIN::Time dropSkillOrbCoolTimer;					// リザルト時のスキルオーブドロップアニメーションで使うタイマー
	friend MultiPlayFlowClientSide;



protected:
	void sort(std::list<CLIENT_DATA_CLIENT_SIDE> &ranking) {
		// ランキングでソート
		ranking.sort(
			[&](CLIENT_DATA_CLIENT_SIDE &a, CLIENT_DATA_CLIENT_SIDE &b) {
				if (a.skillPoint == b.skillPoint) return a.id < b.id;
				else return a.skillPoint < b.skillPoint;
			}
		);
	}

	int get_rank(std::list<CLIENT_DATA_CLIENT_SIDE> &ranking, int id) {
		int rank = 0;
		int addRank = 1;

		// ランクを調べる
		for (auto &c : ranking) {
			if (id == c.id) return rank;
			else rank++;
		}
		// ランクを調べる
		auto preClient = *ranking.begin();
		for (auto &client : ranking) {
			if (id == client.id) return rank;
			else {
				if (client.score == preClient.score) addRank++;
				else {
					rank += addRank;
					addRank = 1;
				}
			}

			preClient = client;
		}

		return -1;
	}

public:
	MultiPlayModeClientSide(MultiMap* map, std::wstring modeName) : map_(map) {
		startTime_ = ini::GetFloat(PARAM_PATH + L"mode.ini", modeName.c_str(), L"startTime");
		resultTime_ = ini::GetFloat(PARAM_PATH + L"mode.ini", modeName.c_str(), L"resultTime");
		dropSkillOrbCoolTimer.Start();
	};
	~MultiPlayModeClientSide() { if (map_) delete map_; }

	virtual void Draw(RESPONSE_PLAYER &players, Vector2 offset) { };
	virtual void DrawStart(RESPONSE_PLAYER &players, Vector2 offset);
	virtual void DrawResult(RESPONSE_PLAYER &players, Vector2 offset);
	virtual void ParseResponse(Storage& in) { };
	virtual void Release(RESPONSE_PLAYER &players) { };


	virtual MULTI_MODE GetMode(void) const = 0;
	MultiMap *GetMap(void) { return map_; }
};
