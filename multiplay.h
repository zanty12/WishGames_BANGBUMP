#pragma once
#include <iostream>
#include "lib/network.h"
#include "lib/vector.h"
#include "mapmngr.h"
#include "player.h"
#include "buffer.h"

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
	};

	int nowMember_ = 0;
	Socket sockfd_;
	Player players_[MAX_MEMBER];
	CLIENT_DATA clients_[MAX_MEMBER] = {};
	Buffer sendBuff = Buffer(1024), sendContentBuff = Buffer(1024), recvBuff = Buffer(1024);

private:
	int Register(Address clientAddr, HEADER *localData, Socket sockfd);

	void Unregister(int id);

	void PlayerUpdate(REQUEST_PLAYER req);

	REQUEST_PLAYER RecvUpdate(void);

	void SendUpdate(void);

	void Update();



	void CreateResponseToClient(Buffer &buff) {

		// 初期化
		buff = nullptr;
		int playerNum = 0;

		// プレイヤー数のカウント
		for (int i = 0; i < MAX_MEMBER; i++) if (0 <= clients_[i].header.id) playerNum++;

		// ヘッダー作成
		buff << playerNum;

		// レスポンス作成（プレイヤー）
		for (int i = 0; i < MAX_MEMBER; i++) {
			// 登録されていないならスキップ
			if (0 > clients_[i].header.id) continue;

			// レスポンス情報の設定
			RESPONSE_PLAYER res;
			res.header = clients_[i].header;
			res.position = players_[i].GetPos();

			buff << res;
		}
	}

	void ParseRequestFromClient(Buffer &buff, REQUEST_PLAYER *req) {
		if (req == nullptr) return;

		buff >> req;
	}

public:
	~MultiServer() { sendBuff.Release(); }

	void OpenTerminal(void);
};

class Client : public Server {
private:
	int id = -1;
	Socket sockfd_;
	Address serverAddr;
	FD readfd_;
	Buffer sendBuff = Buffer(1024), recvBuff = Buffer(1024);

public:
	HEADER header_;
	RESPONSE_PLAYER player;
	RESPONSE_PLAYER players_[MAX_MEMBER - 1];	

	int Register();

	void Unregister();

	void PlayerUpdate(RESPONSE_PLAYER res, RESPONSE_PLAYER reses);

	void RecvUpdate(int waitTime);

	void SendUpdate(void);

	void Update(void);



	void CreateRequestToServer(Buffer &outBuff) {
		// コマンド設定
		header_.command = HEADER::REQUEST_UPDATE;

		REQUEST_PLAYER req;
		req.header = header_;
		req.curInput = Input::GetState(0);
		req.preInput = Input::GetPreviousState(0);

		outBuff << req;
	}

	void ParseResponseFromServer(Buffer &buff, RESPONSE_PLAYER *res) {
		if (res == nullptr) return;

		RESPONSE_PLAYER res__;
		int playerNum = 0;

		// ヘッダーの取得
		buff >> header_;
		buff >> playerNum;


		// レスポンス解析（プレイヤー）
		for (int i = 0; i < playerNum; i++) {
			buff >> res__;

			res[i] = res__;
		}
	}
};
