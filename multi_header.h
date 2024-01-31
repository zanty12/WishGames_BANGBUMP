#pragma once
#include "lib/network.h"
#include "multi_player.h"
#include "multi_animenum.h"
#include "multi_objenum.h"
#include "attribute_type.h"
#include "storage.h"
#include "xinput.h"
#include "attribute.h"

using namespace Network;

enum MULTI_MODE {
	NONE = -1,
	CHARACTER_SELECT = 0,
	AREA_CAPTURE,
	//INTERMEDIATE_RESULT_1,
	//OBSTACLE_RACE,
	//INTERMEDIATE_RESULT_2,
	ENEMY_RUSH,
	//INTERMEDIATE_RESULT_3,
	FINAL_BATTLE,
};


// ヘッダー
struct HEADER {
	enum {
		NONE = -1,
	};

	enum COMMAND {
		REQUEST_LOGIN,
		RESPONSE_LOGIN,
		REQUEST_LOGOUT,
		RESPONSE_LOGOUT,
		REQUEST_UPDATE,
		RESPONSE_UPDATE
	};



	int id = -1;
	int command = NONE;
};
// クライアントデータ（クライアント側）
struct CLIENT_DATA_CLIENT_SIDE {
	int id = -1;												// ID
	ATTRIBUTE_TYPE moveAttributeType = ATTRIBUTE_TYPE_FIRE;		// 移動属性タイプ
	ATTRIBUTE_TYPE attackAttributeType = ATTRIBUTE_TYPE_FIRE;	// 攻撃属性タイプ
	MULTI_ANIMATION_TYPE animType;								// アニメーションタイプ
	Vector2 position;											// 座標
	Vector2 moveVelocity;										// 移動ベロシティ
	Vector2 attackVelocity;										// 攻撃ベロシティ
	Vector2 warpVelocity;										// ワープベロシティ
	int score = 0;												// スコア
	int skillPoint = 0;											// スキルポイント
	int previousSkillPoint = 0;									// 1ゲーム前のスキルポイント
};
// クライアントデータ（サーバー側）
struct CLIENT_DATA_SERVER_SIDE {
	HEADER header;
	Socket sockfd_;
	Address clientAddr_;										// クライアントアドレス
	ATTRIBUTE_TYPE moveAttributeType = ATTRIBUTE_TYPE_FIRE;		// 移動属性タイプ
	ATTRIBUTE_TYPE attackAttributeType = ATTRIBUTE_TYPE_FIRE;	// 攻撃属性タイプ
	ServerPlayer* player_ = nullptr;							// プレイヤー
	bool isSkip = false;										// スキップ
	XINPUT_GAMEPAD currentInput = XINPUT_GAMEPAD();
	XINPUT_GAMEPAD previousInput = XINPUT_GAMEPAD();

	CLIENT_DATA_SERVER_SIDE() = default;
	CLIENT_DATA_SERVER_SIDE(HEADER header, Socket sockfd, Address addr, ServerPlayer *player)
		: header(header), sockfd_(sockfd), clientAddr_(addr), player_(player) { };
};
// オブジェクトデータ（クライアント側）
struct OBJECT_DATA_CLIENT_SIDE {
	enum {
		NONE = -1,
	};



	int id = -1;
	int tag = NONE;
	int anim = NONE;
	Vector2 position;
	Vector2 velocity;
};





// レスポンス（プレイヤー）
struct RESPONSE_PLAYER {
	typedef CLIENT_DATA_CLIENT_SIDE DESC;
	MULTI_MODE mode = MULTI_MODE::NONE;
	float maxTime = 0;
	float time = 0;
	std::list<CLIENT_DATA_CLIENT_SIDE> clients;
	std::list<OBJECT_DATA_CLIENT_SIDE> objects;

	void CreateResponse(Storage& out, int id) {
		// 初期化
		out = nullptr;

		// ヘッダー作成
		HEADER header;
		header.id = id;
		header.command = HEADER::RESPONSE_UPDATE;

		// レスポンス作成（ヘッダー）
		out << header;
		out << mode;
		out << maxTime;
		out << time;
		out << clients.size();
		out << objects.size();

		// レスポンス作成（プレイヤー）
		for (auto& client : clients) {
			// レスポンス情報の設定
			out << client;
		}

		// レスポンス作成（オブジェクト）
		for (auto& object : objects) {
			// レスポンス情報の設定
			out << object;
		}
	}

	void ParseResponse(Storage& in) {
		HEADER recvHeader;
		size_t playerNum = 0;
		size_t objectNum = 0;

		// ヘッダー取得
		in >> recvHeader;
		in >> mode;
		in >> maxTime;
		in >> time;
		in >> playerNum;
		in >> objectNum;
		int current = in.Current();

		// レスポンス解析（プレイヤー）
		for (int i = 0; i < playerNum; i++) {
			CLIENT_DATA_CLIENT_SIDE res;
			in >> res;
			current = in.Current();

			// 自分なら先頭に
			if (res.id == recvHeader.id) {
				clients.push_front(res);
			}
			else {
				clients.push_back(res);
			}
		}

		// レスポンス解析（オブジェクト）
		for (int i = 0; i < objectNum; i++) {
			OBJECT_DATA_CLIENT_SIDE res;
			in >> res;
			current = in.Current();
			objects.push_back(res);
		}
	}

	void AddObjects(MultiBehavior *list){
		for (auto &instance : *list) {
			auto gameObject = instance.Cast<GameObjectServerSide>();
			int id = gameObject->id;
			Vector2 position = gameObject->transform.position;
			Vector2 velocity = gameObject->velocity;
			objects.push_back({
				id,										// ID
				gameObject->GetType(),					// tag
				0,										// animation
				position,								// pos
				velocity								// vel
				}
			);
		}
	}
};
// リクエスト（プレイヤー）
struct REQUEST_PLAYER {
	struct DESC {
		int id;
		XINPUT_GAMEPAD curInput;
		XINPUT_GAMEPAD preInput;
		ATTRIBUTE_TYPE move = ATTRIBUTE_TYPE_FIRE;
		ATTRIBUTE_TYPE action = ATTRIBUTE_TYPE_FIRE;

	};
	DESC input;

	void CreateRequest(Storage& out, int id) {
		// 初期化
		out = nullptr;

		// ヘッダー作成
		HEADER header;
		header.id = id;
		header.command = HEADER::REQUEST_UPDATE;

		// レスポンス作成（ヘッダー）
		out << header;

		// レスポンス作成（入力）
		out << input;
	}

	void ParseRequest(Storage& in) {
		HEADER recvHeader;

		// ヘッダー取得
		in >> recvHeader;


		// レスポンス解析（入力）
		in >> input;

		// 初期化
		in = nullptr;
	}
};



// レスポンス（キャラクター選択）
struct RESPONSE_CHARACTER_SELECT {
	struct DESC {
		int id = -1;						// 属性のID
		ATTRIBUTE_TYPE moveAttributeType;	// 移動属性タイプ
		ATTRIBUTE_TYPE attackAttributeType;	// 攻撃属性タイプ
	};

	std::list<DESC> characters;

	void CreateResponse(Storage &out) {
		// レスポンス作成（ヘッダー）
		out << characters.size();

		// レスポンス作成（ボディ）
		for (DESC &character : characters) {
			out << character;
		}
	}

	void ParseResponse(Storage &in) {
		size_t characterNum;

		// ヘッダー取得
		in >> characterNum;

		// ボディ取得
		for (int i = 0; i < characterNum; i++) {
			DESC area;
			in >> area;
			characters.push_back(area);
		}
	}
};
// レスポンス（占領モード）
struct RESPONSE_AREA_CAPTURE {
	struct DESC {
		Vector2 position;				// 占領の座標
		float radius = 700;				// 占領のスケール
		float captureRatio = 0.0f;		// 占領度合い
		int id = -1;					// 現在占領しているID
	};

	std::list<DESC> areas;

	void CreateResponse(Storage& out) {
		// レスポンス作成（ヘッダー）
		out << areas.size();

		// レスポンス作成（ボディ）
		for (DESC& area : areas) {
			out << area;
		}
	}

	void ParseResponse(Storage& in) {
		size_t areaNum;

		// ヘッダー取得
		in >> areaNum;

		// ボディ取得
		for (int i = 0; i < areaNum; i++) {
			DESC area;
			in >> area;
			areas.push_back(area);
		}
	}
};
//// レスポンス（中間リザルト）
//struct RESPONSE_INTERMEDIATE_RESULT {
//	struct DESC {
//		int previousSkillPoint = 0;
//	};
//
//	std::list<DESC> 
//};