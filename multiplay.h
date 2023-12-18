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
		SELECT_ATTRIBUTE,
		STAGE,
	};

	int id = -1;
	int command = NONE;
	int scene = NONE;
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
		Player *player_;
	};

	int maxID = 0;
	Socket sockfd_;
	std::list<CLIENT_DATA> clients_;
	Storage sendBuff = Storage(1024), sendContentBuff = Storage(1024), recvBuff = Storage(1024);
	MapMngr mapMngr = MapMngr("data/map/1.csv", nullptr);

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


	std::list<CLIENT_DATA>::iterator find(int id) {
		return std::find_if(clients_.begin(), clients_.end(), [&](CLIENT_DATA client) { 
			return client.header.id == id;
			}
		);
	}
	// ���X�|���X�쐬
	void CreateResponseToClient(Storage &buff) {

		// ������
		buff = nullptr;
		int playerNum = 0;

		// �v���C���[���̃J�E���g
		for (auto &client : clients_) {
			if (0 <= client.header.id) playerNum++;
		}

		// �w�b�_�[�쐬
		buff << playerNum;

		// ���X�|���X�쐬�i�v���C���[�j
		for (auto &client : clients_) {
			// ���X�|���X���̐ݒ�
			RESPONSE_PLAYER res;
			res.header = client.header;
			res.position = client.player_->GetPos();

			buff << res;
		}
	}
	// ���N�G�X�g���
	void ParseRequestFromClient(Storage &buff, REQUEST_PLAYER &req) {

		buff >> req;
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
	void Update(RESPONSE_PLAYER res) {
		SetPos(res.position);
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
	RESPONSE_PLAYER player;
	std::list<RESPONSE_PLAYER> players_;
	Object playerAnim = Object(Vector2(0,0), LoadTexture("data/texture/player.png"));
	~Client() { Unregister(); }

	// �o�^
	int Register();
	// ����
	void Unregister();
	// �X�V
	void PlayerUpdate(RESPONSE_PLAYER &res, std::list<RESPONSE_PLAYER> &reses);
	// ��M
	void RecvUpdate(int waitTime, RESPONSE_PLAYER &res, std::list<RESPONSE_PLAYER> &reses);
	// ���M
	void SendUpdate(void);

	void Update(void);




	std::list<RESPONSE_PLAYER>::iterator find(int id) {
		return std::find_if(players_.begin(), players_.end(), [&](RESPONSE_PLAYER client) {
			return client.header.id == id;
			}
		);
	}
	// ���N�G�X�g�쐬
	void CreateRequestToServer(Storage &outBuff) {
		HEADER &header_ = player.header;

		// �R�}���h�ݒ�
		header_.command = HEADER::REQUEST_UPDATE;

		REQUEST_PLAYER req;
		req.header = header_;
		req.curInput = Input::GetState(0);
		req.preInput = Input::GetPreviousState(0);

		outBuff << req;
	}
	// ���X�|���X���
	void ParseResponseFromServer(Storage &buff, std::list<RESPONSE_PLAYER> &reses) {
		RESPONSE_PLAYER res__;
		int playerNum = 0;

		// �w�b�_�[�̎擾
		buff >> player.header;
		buff >> playerNum;


		// ���X�|���X��́i�v���C���[�j
		for (int i = 0; i < playerNum; i++) {
			buff >> res__;

			reses.push_back(res__);
		}
	}
};
