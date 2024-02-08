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
#include "light_effect.h"
#include "multi_effect.h"
#include <fstream>

extern std::wstring ParamPath;
#define MAX_MEMBER (4)
#define PORT (8080)
#define MAX_BUFF (1024 * 10)



using namespace Network;

/*******************************************************
  Server
********************************************************/
class MultiPlayServer {
private:
	static MultiPlayFlowServerSide *gameMode;				// �Q�[�����[�h
	std::list<int> unregistersID;							// �������Ƃ���ID
	int maxID = 0;											// ID�̍ő�l				
	Socket sockfd_;											// �\�P�b�g
	Storage sendBuff = Storage(1024);						// ���M�o�b�t�@
	Storage recvBuff = Storage(1024);						// ��M�o�b�t�@
	StorageLock	lock_;										// ���X�g���b�N
	bool isFinish = false;									// �I�����

public:
	static std::map<int, CLIENT_DATA_SERVER_SIDE> clients_;	// �N���C�A���g�f�[�^



public:
	MultiPlayServer();

	~MultiPlayServer() {
		// ���
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
	void PlayerUpdate(void);
	// ��M
	void RecvUpdate(void);
	// ���M
	void SendUpdate(void);



public:

	void OpenTerminal(void);

	static MultiPlayFlowServerSide *GetGameMode(void) { return gameMode; }

	std::map<int, CLIENT_DATA_SERVER_SIDE> &GetClients(void) { return clients_; }
};


/*******************************************************
  Client
********************************************************/


class MultiPlayClient {
private:
	static int id;										// ID
	static MultiPlayFlowClientSide *gameMode;			// �Q�[�����[�h
	Socket sockfd_;										// �\�P�b�g
	Address serverAddr;									// �A�h���X
	FD readfd_;											// �t�@�C���f�B�X�N���v�^
	Storage sendBuff = Storage(1024);					// ���M�o�b�t�@
	Storage recvBuff = Storage(1024);					// ��M�o�b�t�@

	RESPONSE_PLAYER res_;								// ���X�|���X
	char *recvTmpBuff = nullptr;						// ��M�o�b�t�@�i���i�[�p�j
	LightEffect lightEffect;

	std::thread sendUpdateFunc;							// ���M�֐�
	std::thread recvUpdateFunc;							// ��M�֐�


public:
	static Vector2 offset;								// �J�������W
	bool isFinish = false;								// �I�����
	int texNo = 0;
	ATTRIBUTE_TYPE move_ = ATTRIBUTE_TYPE_FIRE;
	ATTRIBUTE_TYPE action_ = ATTRIBUTE_TYPE_FIRE;
	static std::unordered_map<int, ClientPlayer *> clients;		// �`��p�N���C�A���g
	std::unordered_map<int, GameObjectClientSide *> objects;	// �`��p�I�u�W�F�N�g

	MultiPlayClient();

	~MultiPlayClient() {
		// �X���b�h�I���܂őҋ@
		sendUpdateFunc.join();
		recvUpdateFunc.join();

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
	int Register(std::string serverAddress = "");
	// ����
	void Unregister(void);
	// �X�V
	void PlayerUpdate(void);
	// ��M
	void RecvUpdate(int waitTime);
	// ���M
	void SendUpdate(void);

	void Update(void);



	static int GetID(void) { return id; }

	static MultiPlayFlowClientSide *GetGameMode(void) { return gameMode; }
};
