#pragma once
#include "multi_header.h"
#include "multi_map.h"
#include "time.h"

class MultiPlayFlowServerSide;
class MultiPlayModeServerSide {
protected:
	float maxTime_ = 60.0f;
	float time_ = 0.0f;
	float startTime_ = 15.0f;			// �J�n�܂ł̎���
	float resultTime_ = 15.0f;			// ���ԃ��U���g�̎���
	bool isSkip = false;
	MultiMap *map_ = nullptr;
	friend MultiPlayFlowServerSide;


public:
	MultiPlayModeServerSide(MultiMap *map, std::wstring modeName) : map_(map) {
		startTime_ = ini::GetFloat(PARAM_PATH + L"mode.ini", modeName.c_str(), L"startTime");
		resultTime_ = ini::GetFloat(PARAM_PATH + L"mode.ini", modeName.c_str(), L"resultTime");
		maxTime_ = ini::GetFloat(PARAM_PATH + L"mode.ini", modeName.c_str(), L"timeLimit") + startTime_ + resultTime_;

	}
	~MultiPlayModeServerSide() { if (map_) delete map_; }
	virtual void Update(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) { };
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
	float startTime_ = 15.0f;							// �J�n�܂ł̎���
	float resultTime_ = 15.0f;							// ���ԃ��U���g�̎���
	std::list<CLIENT_DATA_CLIENT_SIDE> beforeClients;	// �Q�[���J�n����̃N���C�A���g�f�[�^
	MultiMap *map_;
	int clientSpawnCount = 0;							// �N���C�A���g�̃X�|�[���J�E���g
	friend MultiPlayFlowClientSide;



protected:
	void sort(std::list<CLIENT_DATA_CLIENT_SIDE> &ranking) {
		// �����L���O�Ń\�[�g
		ranking.sort(
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
	MultiPlayModeClientSide(MultiMap* map, std::wstring modeName) : map_(map) {
		startTime_ = ini::GetFloat(PARAM_PATH + L"mode.ini", modeName.c_str(), L"startTime");
		resultTime_ = ini::GetFloat(PARAM_PATH + L"mode.ini", modeName.c_str(), L"resultTime");
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
