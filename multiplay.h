#pragma once
#include <thread>
#include "lib/network.h"
#include "gamebase.h"
#include "multi_header.h"
#include "multi_mode_flow.h"
#include "multi_connect_renderer.h"
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
class MultiPlayServer : public GameBase {
private:
	int maxID = 0;										// IDの最大値				
	Socket sockfd_;										// ソケット
	std::list<CLIENT_DATA_SERVER_SIDE> clients_;		// クライアントデータ
	Storage sendBuff = Storage(1024);					// 送信バッファ
	Storage recvBuff = Storage(1024);					// 受信バッファ
	MultiPlayFlowServerSide *gameMode = nullptr;		// ゲームモード
	StorageLock	lock_;									// リストロック
	bool isFinish = false;								// 終了状態



public:
	MultiPlayServer();

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
	void PlayerUpdate(void);
	// 受信
	void RecvUpdate(void);
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
	MultiPlayFlowClientSide *gameMode = nullptr;		// ゲームモード
	RESPONSE_PLAYER res_;								// レスポンス
	char *recvTmpBuff = nullptr;						// 受信バッファ（仮格納用）

	ClientGameObject playerObject;
	MultiRenderer *multiRenderer_ = nullptr;			// 描画
	Animator anim;
	std::thread sendUpdateFunc;							// 送信関数
	std::thread recvUpdateFunc;							// 受信関数


public:
	bool isFinish = false;								// 終了状態
	int texNo = 0;
	ATTRIBUTE_TYPE move_ = ATTRIBUTE_TYPE_FIRE;
	ATTRIBUTE_TYPE action_ = ATTRIBUTE_TYPE_FIRE;

	MultiPlayClient();

	~MultiPlayClient() {
		// スレッド終了まで待機
		sendUpdateFunc.join();
		recvUpdateFunc.join();

		// 解放
		delete gameMode;
		delete multiRenderer_;
		delete recvTmpBuff;
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
	void PlayerUpdate(RESPONSE_PLAYER &res);
	// 受信
	void RecvUpdate(int waitTime, RESPONSE_PLAYER &res);
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
