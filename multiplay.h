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
	MapMngr map_ = MapMngr("data/map/1.csv", nullptr);
	bool isListLock = false;
	MultiPlayServerSide* gameMode = nullptr;
	//MULTI_SCENE scene = SELECT;

private:
	// �o�^
	int Register(Address clientAddr, HEADER &header, Socket sockfd);
	// ����
	void Unregister(int id);
	// �S����
	void AllUnregister(void);
	// �X�V
	void PlayerUpdate(REQUEST_PLAYER req);
	// ��M
	REQUEST_PLAYER RecvUpdate(void);
	// ���M
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



class Object : public GameObject {
private:
	Animator anim = Animator(this);
	void Update(void) override {};

public:
	Object(Vector2 pos, int texNo) : GameObject(pos, 0.0f, texNo) {}
	void Update(RESPONSE_PLAYER::DESC res) {
		anim.SetPos(res.position);
	}

	void Draw(void) {
		anim.Draw();
	}
};



class Client : public Server {
private:
	Socket sockfd_;
	Address serverAddr;
	FD readfd_;
	Storage sendBuff = Storage(1024), recvBuff = Storage(1024);
	MultiPlayClientSide *gameMode = nullptr;
	//Renderer renderer_;


public:
	int id = -1;
	//RESPONSE_PLAYER::DESC player;
	Object playerAnim = Object(Vector2(0,0), LoadTexture("data/texture/player.png"));
	MapMngr mapMngr = MapMngr("data/map/1.csv", nullptr);
	Client() {
		gameMode = new MultiPlayAreaCaptureModeClientSide();
	}
	~Client() { Unregister(); delete gameMode; }

	// �o�^
	int Register();
	// ����
	void Unregister();
	// �X�V
	void PlayerUpdate(RESPONSE_PLAYER &res/*, std::list<RESPONSE_PLAYER> &reses*/);
	// ��M
	void RecvUpdate(int waitTime, RESPONSE_PLAYER &res/*, std::list<RESPONSE_PLAYER> &reses*/);
	// ���M
	void SendUpdate(void);

	void Update(void);




	std::list<RESPONSE_PLAYER::DESC>::iterator find(int id, std::list<RESPONSE_PLAYER::DESC> clients) {
		return std::find_if(clients.begin(), clients.end(), [&](RESPONSE_PLAYER::DESC client) {
			return client.id == id;
			}
		);
	}
};
