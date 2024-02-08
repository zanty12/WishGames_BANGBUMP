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
	static MultiPlayFlowServerSide *gameMode;				// ゲームモード
	std::list<int> unregistersID;							// 抜けたときのID
	int maxID = 0;											// IDの最大値				
	Socket sockfd_;											// ソケット
	Storage sendBuff = Storage(1024);						// 送信バッファ
	Storage recvBuff = Storage(1024);						// 受信バッファ
	StorageLock	lock_;										// リストロック
	bool isFinish = false;									// 終了状態

public:
	static std::map<int, CLIENT_DATA_SERVER_SIDE> clients_;	// クライアントデータ



public:
	MultiPlayServer();

	~MultiPlayServer() {
		// 解放
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
	static MultiPlayFlowClientSide *gameMode;			// ゲームモード
	Socket sockfd_;										// ソケット
	Address serverAddr;									// アドレス
	FD readfd_;											// ファイルディスクリプタ
	Storage sendBuff = Storage(1024);					// 送信バッファ
	Storage recvBuff = Storage(1024);					// 受信バッファ

	RESPONSE_PLAYER res_;								// レスポンス
	char *recvTmpBuff = nullptr;						// 受信バッファ（仮格納用）
	LightEffect lightEffect;

	std::thread sendUpdateFunc;							// 送信関数
	std::thread recvUpdateFunc;							// 受信関数


public:
	static Vector2 offset;								// カメラ座標
	bool isFinish = false;								// 終了状態
	int texNo = 0;
	ATTRIBUTE_TYPE move_ = ATTRIBUTE_TYPE_FIRE;
	ATTRIBUTE_TYPE action_ = ATTRIBUTE_TYPE_FIRE;
	static std::unordered_map<int, ClientPlayer *> clients;		// 描画用クライアント
	std::unordered_map<int, GameObjectClientSide *> objects;	// 描画用オブジェクト

	MultiPlayClient();

	~MultiPlayClient() {
		// スレッド終了まで待機
		sendUpdateFunc.join();
		recvUpdateFunc.join();

		//delete gameMode;
		delete recvTmpBuff;
		sendBuff.Release();
		recvBuff.Release();

		// 登録解除
		Unregister();

		// クリーンアップ
		Cleanup();
	}

	// 登録
	int Register(std::string serverAddress = "");
	// 解除
	void Unregister(void);
	// 更新
	void PlayerUpdate(void);
	// 受信
	void RecvUpdate(int waitTime);
	// 送信
	void SendUpdate(void);

	void Update(void);



	static int GetID(void) { return id; }

	static MultiPlayFlowClientSide *GetGameMode(void) { return gameMode; }
};
