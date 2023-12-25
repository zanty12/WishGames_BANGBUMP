#pragma once
#include "lib/network.h"
#include "storage.h"
#include "player.h"
#include "xinput.h"

using namespace Network;

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
	enum SCENE {
		SELECT,
		AREA_CAPTURE
	};


	int id = -1;
	int command = NONE;
	int scene = NONE;
};
// クライアントデータ（クライアント側）
struct CLIENT_DATA_CLIENT_SIDE {
	int id = -1;
	Vector2 position;
	int score = 0;
};
// クライアントデータ（サーバー側）
struct CLIENT_DATA_SERVER_SIDE {
	HEADER header;
	Socket sockfd_;
	Address clientAddr_;
	Player* player_;
	XINPUT_GAMEPAD currentInput = XINPUT_GAMEPAD();
	XINPUT_GAMEPAD previousInput = XINPUT_GAMEPAD();
	ATTRIBUTE_TYPE actionAttribute = ATTRIBUTE_TYPE_FIRE;
	ATTRIBUTE_TYPE moveAttribute = ATTRIBUTE_TYPE_FIRE;
	int score = 0;

	CLIENT_DATA_SERVER_SIDE(HEADER header, Socket sockfd, Address addr, Player *player)
		: header(header), sockfd_(sockfd), clientAddr_(addr), player_(player) { };
};





// レスポンス（プレイヤー）
struct RESPONSE_PLAYER {
	typedef CLIENT_DATA_CLIENT_SIDE DESC;

	std::list<CLIENT_DATA_CLIENT_SIDE> clients;

	void CreateResponse(Storage& out, int id) {
		// 初期化
		out = nullptr;

		// ヘッダー作成
		HEADER header;
		header.id = id;
		header.command = HEADER::RESPONSE_UPDATE;

		// レスポンス作成（ヘッダー）
		out << header;
		out << clients.size();

		// レスポンス作成（プレイヤー）
		for (auto& client : clients) {
			// レスポンス情報の設定
			out << client;
		}
	}

	void ParseResponse(Storage& in) {
		HEADER recvHeader;
		size_t playerNum = 0;

		// ヘッダー取得
		in >> recvHeader;
		in >> playerNum;


		// レスポンス解析（プレイヤー）
		for (int i = 0; i < playerNum; i++) {
			CLIENT_DATA_CLIENT_SIDE res;
			in >> res;

			// 自分なら先頭に
			if (res.id == recvHeader.id) {
				clients.push_front(res);
			}
			else {
				clients.push_back(res);
			}
		}
	}
};
// リクエスト（プレイヤー）
struct REQUEST_PLAYER {
	struct DESC {
		int id;
		XINPUT_GAMEPAD curInput;
		XINPUT_GAMEPAD preInput;
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
