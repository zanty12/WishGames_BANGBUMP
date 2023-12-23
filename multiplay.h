#pragma once
#include <thread>
#include "gamebase.h"
#include "multi_header.h"
#include "multi_area_capture_mode_.h"
#include "multi_client_gameobject.h"
#include "lib/network.h"

#define SERVER_ADDRESS "192.168.0.7"
#define MAX_MEMBER (4)
#define PORT (8080)

using namespace Network;

/*******************************************************
  Server
********************************************************/
class MultiPlayServer : GameBase {
private:
	int maxID = 0;										// ID�̍ő�l				
	Socket sockfd_;										// �\�P�b�g
	std::list<CLIENT_DATA_SERVER_SIDE> clients_;		// �N���C�A���g�f�[�^
	bool isListLock = false;							// �f�[�^�x�[�X�̃��b�N
	Storage sendBuff = Storage(1024);					// ���M�o�b�t�@
	Storage recvBuff = Storage(1024);					// ��M�o�b�t�@
	MultiPlayServerSide *gameMode = nullptr;			// �Q�[�����[�h

public:
	MultiPlayServer() {
		WSAData data;
		Startup(v2_2, data);

		mapmngr_ = new MapMngr("data/map/stage1_test.csv", this);
		gameMode = new MultiPlayAreaCaptureModeServerSide(mapmngr_);
	}

	~MultiPlayServer() {
		// ���
		delete gameMode;
		sendBuff.Release();
		recvBuff.Release();

		// �o�^����
		AllUnregister();

		// �N���[���A�b�v
		Cleanup();
	}

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

	void Update() override;


	std::list<CLIENT_DATA_SERVER_SIDE>::iterator find(int id) {
		return std::find_if(clients_.begin(), clients_.end(), [&](CLIENT_DATA_SERVER_SIDE client) {
			return client.header.id == id;
			}
		);
	}


public:

	void OpenTerminal(void);
};


/*******************************************************
  Client
********************************************************/


class MultiPlayClient : public GameBase {
private:
	int id = -1;										// ID
	Socket sockfd_;										// �\�P�b�g
	Address serverAddr;									// �A�h���X
	FD readfd_;											// �t�@�C���f�B�X�N���v�^
	Storage sendBuff = Storage(1024);					// ���M�o�b�t�@
	Storage recvBuff = Storage(1024);					// ��M�o�b�t�@
	MultiPlayClientSide *gameMode = nullptr;			// �Q�[�����[�h

	ClientGameObject playerObject;
	Animator anim;
	std::thread sendUpdateFunc;							// ���M�֐�


public:
	int texNo = 0;
	MapMngr mapMngr = MapMngr("data/map/1.csv", nullptr);

	MultiPlayClient();

	~MultiPlayClient() {
		// �X���b�h�I���܂őҋ@
		sendUpdateFunc.join();

		// ���
		delete gameMode;
		sendBuff.Release();
		recvBuff.Release();

		// �o�^����
		Unregister();

		// �N���[���A�b�v
		Cleanup();
	}

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
