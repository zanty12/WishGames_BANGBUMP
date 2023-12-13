#pragma once
#include <iostream>
#include <list>
#include <tuple>
#include <algorithm>
#include "lib/network.h"
#include "lib/vector.h"
#include "mapmngr.h"
#include "player.h"
#include "storage.h"
#include "xinput.h"

#define SERVER_ADDRESS "10.192.121.53"
#define MAX_MEMBER (4)
#define PORT (8080)

using namespace Network;

struct HEADER {
	enum COMMAND {
		NONE,
		REQUEST_LOGIN,
		RESPONSE_LOGIN,
		REQUEST_LOGOUT,
		RESPONSE_LOGOUT,
		REQUEST_UPDATE,
		RESPONSE_UPDATE
	};

	int command = NONE;
	int id = -1;
};
struct OTHER_CLIENT {
	HEADER header;
	Vector2 position;
};

struct REQUEST_PLAYER {
	HEADER header;
	XINPUT_GAMEPAD curInput;
	XINPUT_GAMEPAD preInput;
};

struct RESPONSE_PLAYER {
	HEADER header;
	Vector2 position;
};


class MultiServer : Server {
private:
	struct CLIENT_DATA {
		HEADER header;
		Socket sockfd_;
		Address clientAddr_;
		Player player_;
	};

	int maxID = 0;
	Socket sockfd_;
	std::list<CLIENT_DATA> clients_;
	Storage sendBuff = Storage(1024), sendContentBuff = Storage(1024), recvBuff = Storage(1024);
	MapMngr mapMngr;

private:
	int Register(Address clientAddr, HEADER &header, Socket sockfd);

	void Unregister(int id);

	void PlayerUpdate(REQUEST_PLAYER req);

	REQUEST_PLAYER RecvUpdate(void);

	void SendUpdate(void);

	void Update();


	std::list<CLIENT_DATA>::iterator find(int id) {
		return std::find(clients_.begin(), clients_.end(), [&](CLIENT_DATA client) { 
			return client.header.id == id;
			}
		);
	}

	void CreateResponseToClient(Storage &buff) {

		// 初期化
		buff = nullptr;
		int playerNum = 0;

		// プレイヤー数のカウント
		for (auto client : clients_) {
			if (0 <= client.header.id) playerNum++;
		}

		// ヘッダー作成
		buff << playerNum;

		// レスポンス作成（プレイヤー）
		for (auto client : clients_) {
			// レスポンス情報の設定
			RESPONSE_PLAYER res;
			res.header = client.header;
			res.position = client.player_.GetPos();

			buff << res;
		}
	}

	void ParseRequestFromClient(Storage &buff, REQUEST_PLAYER *req) {
		if (req == nullptr) return;

		buff >> req;
	}

public:
	~MultiServer() { sendBuff.Release(); }

	void OpenTerminal(void);
};

class Client : public Server {
private:
	Socket sockfd_;
	Address serverAddr;
	FD readfd_;
	Storage sendBuff = Storage(1024), recvBuff = Storage(1024);

public:
	RESPONSE_PLAYER player;
	std::list<RESPONSE_PLAYER> players_;

	int Register();

	void Unregister();

	void PlayerUpdate(RESPONSE_PLAYER &res, std::list<RESPONSE_PLAYER> &reses);

	void RecvUpdate(int waitTime, RESPONSE_PLAYER &res, std::list<RESPONSE_PLAYER> &reses);

	void SendUpdate(void);

	void Update(void);




	std::list<RESPONSE_PLAYER>::iterator find(int id) {
		return std::find(players_.begin(), players_.end(), [&](RESPONSE_PLAYER client) {
			return client.header.id == id;
			}
		);
	}

	void CreateRequestToServer(Storage &outBuff) {
		HEADER &header_ = player.header;

		// コマンド設定
		header_.command = HEADER::REQUEST_UPDATE;

		REQUEST_PLAYER req;
		req.header = header_;
		req.curInput = Input::GetState(0);
		req.preInput = Input::GetPreviousState(0);

		outBuff << req;
	}

	void ParseResponseFromServer(Storage &buff, std::list<RESPONSE_PLAYER> &reses) {
		RESPONSE_PLAYER res__;
		int playerNum = 0;

		// ヘッダーの取得
		buff >> player.header;
		buff >> playerNum;


		// レスポンス解析（プレイヤー）
		for (int i = 0; i < playerNum; i++) {
			buff >> res__;

			reses.push_back(res__);
		}
	}
};
