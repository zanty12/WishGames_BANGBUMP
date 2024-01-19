#pragma once
#include <thread>
#include <unordered_map>
#include "lib/network.h"
#include "gamebase.h"
#include "multi_header.h"
#include "multi_map.h"
#include "multi_mode_flow.h"
#include "multi_connect_renderer.h"
#include "multi_player.h"
#include "storage_lock.h"
#include <fstream>

extern std::string SERVER_ADDRESS;
//#define SERVER_ADDRESS "192.168.0.7"
#define MAX_MEMBER (4)
#define PORT (8080)
#define MAX_BUFF (4096)

using namespace Network;

/*******************************************************
  Server
********************************************************/
class MultiPlayServer {
private:
	int maxID = 0;											// ID�̍ő�l				
	Socket sockfd_;											// �\�P�b�g
	Storage sendBuff = Storage(1024);						// ���M�o�b�t�@
	Storage recvBuff = Storage(1024);						// ��M�o�b�t�@
	MultiPlayFlowServerSide *gameMode = nullptr;			// �Q�[�����[�h
	StorageLock	lock_;										// ���X�g���b�N
	bool isFinish = false;									// �I�����
	MultiMap map;											// �}�b�v

public:
	static std::map<int, CLIENT_DATA_SERVER_SIDE> clients_;	// �N���C�A���g�f�[�^



public:
	MultiPlayServer();

	~MultiPlayServer() {
		// ���
		sendBuff.Release();
		recvBuff.Release();
		map.Release();

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
	void PlayerUpdate(void);
	// ��M
	void RecvUpdate(void);
	// ���M
	void SendUpdate(void);



public:

	void OpenTerminal(void);

	std::map<int, CLIENT_DATA_SERVER_SIDE> &GetClients(void) { return clients_; }
};


/*******************************************************
  Client
********************************************************/


class MultiPlayClient {
private:
	int id = -1;										// ID
	Socket sockfd_;										// �\�P�b�g
	Address serverAddr;									// �A�h���X
	FD readfd_;											// �t�@�C���f�B�X�N���v�^
	Storage sendBuff = Storage(1024);					// ���M�o�b�t�@
	Storage recvBuff = Storage(1024);					// ��M�o�b�t�@
	MultiPlayFlowClientSide *gameMode = nullptr;		// �Q�[�����[�h
	RESPONSE_PLAYER res_;								// ���X�|���X
	char *recvTmpBuff = nullptr;						// ��M�o�b�t�@�i���i�[�p�j
	MultiMap map;										// �}�b�v
	std::unordered_map<int, ClientPlayer*> clients;		// �`��p�N���C�A���g
	std::unordered_map<int, ClientGameObject*> objects;	// �`��p�I�u�W�F�N�g

	std::thread sendUpdateFunc;							// ���M�֐�
	std::thread recvUpdateFunc;							// ��M�֐�


public:
	static Vector2 offset;								// �J�������W
	bool isFinish = false;								// �I�����
	int texNo = 0;
	ATTRIBUTE_TYPE move_ = ATTRIBUTE_TYPE_FIRE;
	ATTRIBUTE_TYPE action_ = ATTRIBUTE_TYPE_FIRE;

	MultiPlayClient();

	~MultiPlayClient() {
		// �X���b�h�I���܂őҋ@
		sendUpdateFunc.join();
		recvUpdateFunc.join();

		// ���
		map.Release();
		//delete gameMode;
		delete recvTmpBuff;
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
	void PlayerUpdate(RESPONSE_PLAYER &res);
	// ��M
	void RecvUpdate(int waitTime, RESPONSE_PLAYER &res);
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
