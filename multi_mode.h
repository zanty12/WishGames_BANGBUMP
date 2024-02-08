#pragma once
#include "multi_header.h"
#include "multi_runenum.h"
#include "multi_map.h"
#include "multi_result_skillorb.h"
#include "sound.h"
#include "time.h"

/***********************************************************
	Server
************************************************************/
class MultiPlayFlowServerSide;
class MultiPlayModeServerSide {
protected:
	float maxTime_ = 60.0f;
	float time_ = 0.0f;
	float startTime_ = 15.0f;			// 開始までの時間
	float playTime_ = 15.0f;			// プレイの時間
	float resultTime_ = 15.0f;			// 中間リザルトの時間
	bool isSkip = false;				// スキップ
	bool isPlayerMove = false;			// プレイヤーの移動制限
	MultiMap *map_ = nullptr;
	friend MultiPlayFlowServerSide;


public:
	enum MODE {
		START,
		PLAY,
		RESULT,
	};

	int mode = NONE;
	int preMode = NONE;

	MultiPlayModeServerSide(std::wstring modeName) {
		startTime_ = ini::GetFloat(PARAM_PATH + L"mode.ini", modeName.c_str(), L"startTime");
		resultTime_ = ini::GetFloat(PARAM_PATH + L"mode.ini", modeName.c_str(), L"resultTime");
		playTime_ = ini::GetFloat(PARAM_PATH + L"mode.ini", modeName.c_str(), L"playTime");
		maxTime_ = (0.0f < playTime_ ? playTime_ : -playTime_) + startTime_ + resultTime_;

		std::string mapPath = ini::GetString(PARAM_PATH + L"mode.ini", modeName.c_str(), L"path");
		map_ = new MultiMap(MAP_PATH + mapPath, MULTIPLAY_RUN_TYPE_SERVER);
	}
	~MultiPlayModeServerSide() { if (map_) delete map_; }
	virtual void Update(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) { };
	virtual void UpdateStart(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients);
	virtual void UpdateResult(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients);
	virtual void CreateResponse(Storage& out) { };
	virtual void Release(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients);

	virtual bool IsPlayerMove(void) const { return isPlayerMove; };
	virtual MULTI_MODE GetMode(void) const = 0;
	float GetTime(void) const { return time_; }
	float GetMaxTime(void) const { return maxTime_; }
	MultiMap *GetMap(void) { return map_; }
};




/***********************************************************
	Client
************************************************************/
class MultiPlayFlowClientSide;
class MultiPlayModeClientSide {
protected:
	float startTime_ = 15.0f;							// 開始までの時間
	float playTime_ = 15.0f;							// プレイの時間
	float resultTime_ = 15.0f;							// 中間リザルトの時間
	MultiMap *map_;
	int clientSpawnCount = 0;							// クライアントのスポーンカウント
	std::list<ResultSkillOrb> rstSkillOrb;				// リザルト時のスキルオーブ
	WIN::Time dropSkillOrbCoolTimer;					// リザルト時のスキルオーブドロップアニメーションで使うタイマー
	bool isNoTimer = false;								// 時間計測をしない
	friend MultiPlayFlowClientSide;


public:
	int soNo = -1;										// BGM


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

	int get_rank(std::unordered_map<int, ClientPlayer*> &ranking, int id) {
		int rank = 0;
		int addRank = 1;
		
		std::list<ClientPlayer *> sortClients;
		for (auto &kvp : ranking) sortClients.push_back(kvp.second);

		// ソート
		sortClients.sort(
			[](ClientPlayer *a, ClientPlayer *b) {
				return a->score > b->score;
			}
		);

		auto preClient = ranking.begin()->second;
		for (auto &client : sortClients) {
			if (id == client->id) return rank;
			else {
				if (client->score == preClient->score) addRank++;
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
	MultiPlayModeClientSide(std::wstring modeName) {
		startTime_ = ini::GetFloat(PARAM_PATH + L"mode.ini", modeName.c_str(), L"startTime");
		resultTime_ = ini::GetFloat(PARAM_PATH + L"mode.ini", modeName.c_str(), L"resultTime");
		playTime_ = ini::GetFloat(PARAM_PATH + L"mode.ini", modeName.c_str(), L"playTime");
		isNoTimer = ini::GetBool(PARAM_PATH + L"mode.ini", modeName.c_str(), L"deathAP");
		dropSkillOrbCoolTimer.Start();

		std::string mapPath = ini::GetString(PARAM_PATH + L"mode.ini", modeName.c_str(), L"path");
		map_ = new MultiMap(MAP_PATH + mapPath, MULTIPLAY_RUN_TYPE_CLIENT);
	};
	~MultiPlayModeClientSide() { if (map_) delete map_; }

	virtual void Draw(RESPONSE_PLAYER &players, Vector2 offset) { };
	virtual void DrawStart(RESPONSE_PLAYER &players, Vector2 offset);
	virtual void DrawResult(RESPONSE_PLAYER &players, Vector2 offset);
	virtual void ParseResponse(Storage& in) { };
	virtual void Release(RESPONSE_PLAYER &players) {};


	virtual MULTI_MODE GetMode(void) const = 0;
	MultiMap *GetMap(void) { return map_; }
};
