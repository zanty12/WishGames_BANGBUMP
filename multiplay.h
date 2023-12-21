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
#include "multi_header.h"
#include "renderer.h"
#include "multi_area_capture_mode_.h"
#include "game.h"

#define SERVER_ADDRESS "192.168.0.121"
#define MAX_MEMBER (4)
#define PORT (8080)

using namespace Network;


class MultiServer : Server {
private:


	int maxID = 0;
	Socket sockfd_;
	std::list<CLIENT_DATA_SERVER_SIDE> clients_;
	Storage sendBuff = Storage(1024), sendContentBuff = Storage(1024), recvBuff = Storage(1024);
	bool isListLock = false;
	MultiPlayServerSide* gameMode = nullptr;
	MapMngr map_ = MapMngr("data/map/1.csv", nullptr);
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


	std::list<CLIENT_DATA_SERVER_SIDE>::iterator find(int id) {
		return std::find_if(clients_.begin(), clients_.end(), [&](CLIENT_DATA_SERVER_SIDE client) { 
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






class Client : public Server {
private:
	Socket sockfd_;
	Address serverAddr;
	FD readfd_;
	Storage sendBuff = Storage(1024), recvBuff = Storage(1024);
	MultiPlayClientSide *gameMode = nullptr;
	//Renderer renderer_;
	Animator anim;


public:
	int id = -1;
	//RESPONSE_PLAYER::DESC player;
	int texNo = 0;
	MapMngr mapMngr = MapMngr("data/map/1.csv", nullptr);
	Client() : texNo(LoadTexture("data/texture/player.png")), anim(Animator(Vector2(0, 0), Vector2(50, 50), texNo)) {
		gameMode = new MultiPlayAreaCaptureModeClientSide(&mapMngr);
	}
	~Client() { Unregister(); delete gameMode; }

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
