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

#define SERVER_ADDRESS "192.168.0.121"
#define MAX_MEMBER (4)
#define PORT (8080)

using namespace Network;


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
	};


	int id = -1;
	int command = NONE;
	int scene = NONE;
};
struct OTHER_CLIENT {
	HEADER header;
	Vector2 position;
};


struct RESPONSE_PLAYER {
	struct DESC {
		int id;
		Vector2 position;
	};
	std::list<DESC> clients;

	void CreateResponse(Storage &out, int id) {
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
		for (auto &client : clients) {
			// レスポンス情報の設定
			out << client;
		}
	}

	void ParseResponse(Storage &in) {
		HEADER recvHeader;
		size_t playerNum = 0;

		// ヘッダー取得
		in >> recvHeader;
		in >> playerNum;


		// レスポンス解析（プレイヤー）
		for (int i = 0; i < playerNum; i++) {
			DESC res;
			in >> res;

			// 自分なら先頭に
			if (res.id == recvHeader.id) {
				clients.push_front(res);
			}
			else {
				clients.push_back(res);
			}
		}

		// 初期化
		in = nullptr;
	}
};

struct REQUEST_PLAYER {
	struct DESC {
		int id;
		XINPUT_GAMEPAD curInput;
		XINPUT_GAMEPAD preInput;
	};
	DESC input;

	void CreateRequest(Storage &out, int id) {
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

	void ParseRequest(Storage &in) {
		HEADER recvHeader;

		// ヘッダー取得
		in >> recvHeader;


		// レスポンス解析（入力）
		in >> input;

		// 初期化
		in = nullptr;
	}
};


class MultiServer : Server {
private:
	struct CLIENT_DATA {
		HEADER header;
		Socket sockfd_;
		Address clientAddr_;
		Player *player_;
	};

	int maxID = 0;
	Socket sockfd_;
	std::list<CLIENT_DATA> clients_;
	Storage sendBuff = Storage(1024), sendContentBuff = Storage(1024), recvBuff = Storage(1024);
	MapMngr mapMngr = MapMngr("data/map/1.csv", nullptr);
	//MULTI_SCENE scene = SELECT;

private:
	// 登録
	int Register(Address clientAddr, HEADER &header, Socket sockfd);
	// 解除
	void Unregister(int id);
	// 全解除
	void AllUnregister(void);
	// 更新
	void PlayerUpdate(REQUEST_PLAYER req);
	// 受信
	REQUEST_PLAYER RecvUpdate(void);
	// 送信
	void SendUpdate(void);

	void Update();


	std::list<CLIENT_DATA>::iterator find(int id) {
		return std::find_if(clients_.begin(), clients_.end(), [&](CLIENT_DATA client) { 
			return client.header.id == id;
			}
		);
	}


public:
	~MultiServer() { 
		sendBuff.Release();
		sendContentBuff.Release();
		recvBuff.Release();
		AllUnregister();
	}

	void OpenTerminal(void);
};



class Object : public GameObject {
private:
	Animator anim = Animator(this);
	void Update(void) override {};

public:
	Object(Vector2 pos, int texNo) : GameObject(pos, 0.0f, texNo) {}
	void Update(RESPONSE_PLAYER::DESC res) {
		anim.SetPos((res.position / 2 + Vector2(1600, 900) / 2));
	}

	void Draw(void) override {
		anim.Draw();
	}
};




class Client : public Server {
private:
	Socket sockfd_;
	Address serverAddr;
	FD readfd_;
	Storage sendBuff = Storage(1024), recvBuff = Storage(1024);

public:
	int id = -1;
	//RESPONSE_PLAYER::DESC player;
	Object playerAnim = Object(Vector2(0,0), LoadTexture("data/texture/player.png"));
	~Client() { Unregister(); }

	// 登録
	int Register();
	// 解除
	void Unregister();
	// 更新
	void PlayerUpdate(RESPONSE_PLAYER &res/*, std::list<RESPONSE_PLAYER> &reses*/);
	// 受信
	void RecvUpdate(int waitTime, RESPONSE_PLAYER &res/*, std::list<RESPONSE_PLAYER> &reses*/);
	// 送信
	void SendUpdate(void);

	void Update(void);




	std::list<RESPONSE_PLAYER::DESC>::iterator find(int id, std::list<RESPONSE_PLAYER::DESC> clients) {
		return std::find_if(clients.begin(), clients.end(), [&](RESPONSE_PLAYER::DESC client) {
			return client.id == id;
			}
		);
	}
};
