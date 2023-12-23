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
	int maxID = 0;										// IDの最大値				
	Socket sockfd_;										// ソケット
	std::list<CLIENT_DATA_SERVER_SIDE> clients_;		// クライアントデータ
	bool isListLock = false;							// データベースのロック
	Storage sendBuff = Storage(1024);					// 送信バッファ
	Storage recvBuff = Storage(1024);					// 受信バッファ
	MultiPlayServerSide *gameMode = nullptr;			// ゲームモード

public:
	MultiPlayServer() {
		WSAData data;
		Startup(v2_2, data);

		mapmngr_ = new MapMngr("data/map/stage1_test.csv", this);
		gameMode = new MultiPlayAreaCaptureModeServerSide(mapmngr_);
	}

	~MultiPlayServer() {
		// 解放
		delete gameMode;
		sendBuff.Release();
		recvBuff.Release();

		// 登録解除
		AllUnregister();

		// クリーンアップ
		Cleanup();
	}

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
	Socket sockfd_;										// ソケット
	Address serverAddr;									// アドレス
	FD readfd_;											// ファイルディスクリプタ
	Storage sendBuff = Storage(1024);					// 送信バッファ
	Storage recvBuff = Storage(1024);					// 受信バッファ
	MultiPlayClientSide *gameMode = nullptr;			// ゲームモード

	ClientGameObject playerObject;
	Animator anim;
	std::thread sendUpdateFunc;							// 送信関数


public:
	int texNo = 0;
	MapMngr mapMngr = MapMngr("data/map/1.csv", nullptr);

	MultiPlayClient();

	~MultiPlayClient() {
		// スレッド終了まで待機
		sendUpdateFunc.join();

		// 解放
		delete gameMode;
		sendBuff.Release();
		recvBuff.Release();

		// 登録解除
		Unregister();

		// クリーンアップ
		Cleanup();
	}

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
