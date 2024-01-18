#define _CRT_SECURE_NO_WARNINGS
#include "multiplay.h"
#include "xinput.h"
#include "time.h"
#include <windows.h>
#include <thread>
#pragma comment(lib, "lib/lib.lib")

//#define DEBUG_INPUT
//#define DEBUG_LOCKED
//#define DEBUG_SENDLEN
std::string SERVER_ADDRESS;

MultiPlayServer::MultiPlayServer() {
	WSAData data;
	Startup(v2_2, data);
	map.Load("data/map/MultiPlay_Map1.csv");

	//gameMode = new MultiPlayFlowServerSide(this);
}

int MultiPlayServer::Register(Address clientAddr, HEADER &header, Socket sockfd) {
	// ロック
	lock_.Lock();
#ifdef DEBUG_LOCKED
	std::cout << "REGISTER LOCK";
#endif

	// プレイヤー作成
	Vector2 pos = Vector2(200, 200);
	float rot = 0.0f;
	Vector2 vel = Vector2::Zero;
	ServerPlayer *player = new ServerPlayer();
	player->transform.position = *map.startPosition.begin();

	// ヘッダーの更新
	header.command = HEADER::RESPONSE_LOGIN;
	header.id = maxID++;

	// クライアントデータの作成
	CLIENT_DATA_SERVER_SIDE clientData = {
		header,
		sockfd,
		clientAddr,
		player
	};


	// プレイヤー追加
	clients_[header.id] = clientData;


	// 送信
	header.command = HEADER::RESPONSE_LOGIN;
	SendTo(this->sockfd_, (char *)&header, sizeof(HEADER), 0, clientAddr);
	std::cout << "Res >> ID:" << (maxID - 1) << " Login" << std::endl;



	// ロック解除
#ifdef DEBUG_LOCKED
	std::cout << " - REGISTER UNLOCK" << std::endl;
#endif
	lock_.Unlock();

	return maxID - 1;
}

void MultiPlayServer::Unregister(int id) {
	// ロック
	lock_.Lock();
#ifdef DEBUG_LOCKED
	std::cout << "UNREGISTER LOCK";
#endif


	// 解除
	clients_[id].header.command = HEADER::RESPONSE_LOGOUT;
	clients_[id].sockfd_.Close();

	// 送信
	SendTo(sockfd_, (char *)&clients_[id].header, sizeof(HEADER), 0, clients_[id].clientAddr_);
	std::cout << "Res >> ID:" << " Logout" << std::endl;

	// 削除
	delete clients_[id].player_;
	clients_.erase(id);



	// ロック解除
#ifdef DEBUG_LOCKED
	std::cout << " - UNREGISTER UNLOCK" << std::endl;
#endif
	lock_.Unlock();
}

void MultiPlayServer::AllUnregister(void) {
	while (clients_.size())
	{
		Unregister(clients_.begin()->second.header.id);
	}
}

void MultiPlayServer::PlayerUpdate(void) {
	// ロック
	lock_.Lock();
#ifdef DEBUG_LOCKED
	std::cout << "UPD LOCK";
#endif

	for (auto &kvp : clients_) {
		auto &client = kvp.second;
		auto &player = client.player_;

		Input::SetState(0, client.currentInput);
		Input::SetPreviousState(0, client.previousInput);
		player->Update();
		Vector2 normal;
		int type = map.Collision(player->transform.position, player->radius, &normal);
		// ヒットしたなら反射
		if (type >= 0) {
			float t = Vector2::Dot(normal, -player->velocity);
			player->velocity = 2 * t * normal + player->velocity;
		}
		player->transform.position += player->velocity;

#ifdef DEBUG_INPUT
		std::cout << Input::GetStickLeft(0).x << ", " << Input::GetStickLeft(0).y << std::endl;
#endif
	}
	

	// ロック解除
#ifdef DEBUG_LOCKED
	std::cout << " - UPD UNLOCK" << std::endl;
#endif
	lock_.Unlock();
}

void MultiPlayServer::RecvUpdate(void) {
	// リクエスト情報を書き出す
	REQUEST_PLAYER req;
	req.ParseRequest(recvBuff);



	// ロック
	lock_.Lock();
#ifdef DEBUG_LOCKED
	std::cout << "RCV LOCK";
#endif

	// プレイヤーの検索
	auto iterator = clients_.find(req.input.id);

	// 検索したなら
	if (iterator != clients_.end()) {
		CLIENT_DATA_SERVER_SIDE &data = iterator->second;
		// 入力情報を設定
		data.currentInput = req.input.curInput;
		data.previousInput = req.input.preInput;

		// 属性を設定
		data.moveAttribute = req.input.move;
		data.actionAttribute = req.input.action;

#ifdef DEBUG_INPUT
		Input::SetState(1, data.currentInput);
		Input::SetPreviousState(1, data.previousInput);
		std::cout << Input::GetStickLeft(1).x << ", " << Input::GetPreviousStickLeft(1).y << std::endl;
#endif
	}

	// ロック解除
#ifdef DEBUG_LOCKED
	std::cout << " - RCV UNLOCK" << std::endl;
#endif
	lock_.Unlock();
}

void MultiPlayServer::SendUpdate(void) {
	DWORD startTime, currentTime, onceFrameTime;
	startTime = currentTime = timeGetTime();
	onceFrameTime = 1000 / 60;

	while (!isFinish) {

		currentTime = timeGetTime();

		if (currentTime - startTime > onceFrameTime) {
			startTime = currentTime;

			// ロック
			lock_.Lock();
#ifdef DEBUG_LOCKED
			std::cout << "SND LOCK";
#endif

			// レスポンスの作成
			RESPONSE_PLAYER res;

			//// レスポンス情報の登録
			static float time = 0.0f;
			time += 1;
			res.time = time;
			//res.mode = gameMode->GetMode();
			//res.maxTime = gameMode->GetMaxTime();
			//res.time = gameMode->GetTime();

			// クライアント情報の登録
			for (auto &client : clients_) {
				auto &player = client.second;
				res.clients.push_back({ 
					player.header.id, 
					player.moveAttribute, player.actionAttribute,
					player.player_->transform.position, Color::White,
					0, 0, 0}
				);
			}

			//// オブジェクト情報の登録
			//for (auto &skillorb : orb_mngr_->GetSkillOrbs()) {
			//	if (skillorb->GetDiscard()) continue;
			//	int id = std::atoi(skillorb->GetID().c_str());
			//	Vector2 position = skillorb->GetPos();
			//	float rotation = skillorb->GetRot();
			//	Vector2 scale = skillorb->GetScl();
			//	res.objects.push_back({
			//		id,										// ID
			//		OBJECT_DATA_CLIENT_SIDE::SKILL_POINT,	// tag
			//		0,										// animation
			//		position,								// pos
			//		rotation,								// rot
			//		scale									// scl
			//		}
			//	);
			//}

			// クライアント全員に送信する
			for (auto &kvp : clients_) {
				auto &client = kvp.second;

				// 登録されていないならスキップ
				if (client.header.id < 0) continue;

				// 宛先の登録とレスポンス内容の結合
				res.CreateResponse(sendBuff, client.header.id);

				// ゲームモードのレスポンス内容の結合
				//gameMode->CreateResponse(sendBuff);

#ifdef DEBUG_SENDLEN
				std::cout << "SENDBUFF : " << sendBuff.Length() << std::endl;
#endif

				// 送信
				SendTo(sockfd_, sendBuff, sendBuff.Length(), 0, client.clientAddr_);
			}

			// ロック解除
#ifdef DEBUG_LOCKED
			std::cout << " - SND UNLOCK" << std::endl;
#endif
			lock_.Unlock();
		}
	}
}


void MultiPlayServer::OpenTerminal(void) {
	// ソケット作成
	sockfd_ = Socket(AddressFamily::IPV4, Type::UDP, 0);
	// バインド用アドレス作成
	Address addr = Address(AddressFamily::IPV4, INADDR_ANY, PORT);
	// バインド
	sockfd_.Bind(addr);

	DWORD startTime, currentTime, onceFrameTime;
	startTime = currentTime = timeGetTime();
	onceFrameTime = 1000 / 60;


	// SendUpdate()をスレッドを立てて関数を呼び出す
	std::thread sendUpdateFunc(&MultiPlayServer::SendUpdate, this);



	MSG msg;
	while (true) {
		// メッセージ
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				isFinish = true;
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		// 受信
		{
			FD fd;
			fd.Add(sockfd_);
			Select(&fd, nullptr, nullptr, 0, 1);
			if (fd.Contains(sockfd_)) {
				Address clientAddr;
				int clientAddrLen = sizeof(clientAddr);
				char buff[MAX_BUFF] = {};



				// 受信
				int buffLen = RecvFrom(sockfd_, buff, MAX_BUFF, 0, &clientAddr, &clientAddrLen);

				if (0 <= buffLen) {
					HEADER *pHeader = (HEADER *)buff;

					switch (pHeader->command)
					{
					case HEADER::COMMAND::REQUEST_LOGIN: {
						// 登録
						std::cout << "Req << ID:*" << " Login" << std::endl;
						int id = Register(clientAddr, *pHeader, 0);
						// 送信
						std::cout << id << " : 登録しました" << std::endl;
						break;
					};
					case HEADER::COMMAND::REQUEST_LOGOUT: {
						// 解除
						std::cout << "Req << ID:" << pHeader->id << " Logout" << std::endl;
						Unregister(pHeader->id);
						std::cout << pHeader->id << " : 解除しました" << std::endl;
						break;
					}
					case HEADER::COMMAND::REQUEST_UPDATE: {
						// 更新
						recvBuff.Push(buff, buffLen);
						RecvUpdate();
						break;
					}
					}
				}
			}
		}

		// 更新
		{
			if (clients_.size()) {
				currentTime = timeGetTime();
				if (currentTime - startTime > onceFrameTime) {
					startTime = currentTime;

					PlayerUpdate();
				}
			}
		}

		if (GetAsyncKeyState(VK_ESCAPE)) {
			isFinish = true;
			break;
		}

		Time::Update();

		recvBuff = nullptr;
		sendBuff = nullptr;
	}

	sendUpdateFunc.join();
}











MultiPlayClient::MultiPlayClient() : texNo(LoadTexture("data/texture/player.png"))/*, anim(Animator(&playerObject, 2, true, 1, 1, 1))*/ {
	WSAData data;
	Startup(v2_2, data);

	//gameMode = new MultiPlayFlowClientSide(this);

	map.Initialize();
	map.Load("data/map/MultiPlay_Map1.csv");

	// スレッドを立てる
	sendUpdateFunc = std::thread(&MultiPlayClient::SendUpdate, this);
	recvUpdateFunc = std::thread(&MultiPlayClient::Update, this);

	// 受信用領域を確保する
	recvTmpBuff = new char[MAX_BUFF];

	// IPV4アドレスの登録
	wchar_t addr_w[128] = {};
	char addr[256] = {};
	GetPrivateProfileString(L"System", L"Addr", L"0.0.0.0", addr_w, 128, L"data/multiplay.ini");
	wcstombs(addr, addr_w, 128);
	SERVER_ADDRESS = addr;
}

int MultiPlayClient::Register() {
	HEADER header;

	// ソケット作成
	sockfd_ = Socket(AddressFamily::IPV4, Type::UDP, 0);

	// アドレス作成
	serverAddr = Address(AddressFamily::IPV4, SERVER_ADDRESS.c_str(), PORT);

	// コマンド設定
	header.command = HEADER::COMMAND::REQUEST_LOGIN;

	// 操作
	readfd_.Add(sockfd_);

	// 送信
	std::cout << "Req >> ID:*" << " Login" << std::endl;
	SendTo(sockfd_, (char *)&header, sizeof(HEADER), 0, serverAddr);

	// 受信
	while (true) {
		Recv(sockfd_, (char *)&header, sizeof(HEADER), 0);
		if (header.command = HEADER::RESPONSE_LOGIN) break;
	}

	// IDを記録
	id = header.id;

	std::cout << "Res << ID:" << header.id << " Login" << std::endl;
	std::cout << header.id << "番目に登録しました。" << std::endl;

	return header.id;
}

void MultiPlayClient::Unregister() {
	HEADER header;

	// コマンド設定
	header.command = HEADER::REQUEST_LOGOUT;
	// ID設定
	header.id = id;

	// 送信
	std::cout << "Req << ID:" << header.id << " Logout" << std::endl;
	SendTo(sockfd_, (char *)&header, sizeof(HEADER), 0, serverAddr);

	std::cout << id << "を解除しました。" << std::endl;

	sockfd_.Close();
}

void MultiPlayClient::PlayerUpdate(RESPONSE_PLAYER &res) {
	map.Draw(Vector2(0, 0));
	for (auto &client : res.clients) {
		DrawSprite(texNo, client.position, 0.0, Vector2::One * 100, Color::White);
	}
}

void MultiPlayClient::SendUpdate(void) {
	DWORD startTime, currentTime, onceFrameTime;
	startTime = currentTime = timeGetTime();
	onceFrameTime = 1000 / 60;

	while (!isFinish) {
		currentTime = timeGetTime();

		if (currentTime - startTime > onceFrameTime) {
			startTime = currentTime;
			// リクエストの作成
			REQUEST_PLAYER req;
			req.input = { id, Input::GetState(0), Input::GetPreviousState(0), move_, action_ };
			req.CreateRequest(sendBuff, id);

			// 送信
			SendTo(sockfd_, sendBuff, sendBuff.Length(), 0, serverAddr);
			sendBuff = nullptr;

			// 入力更新
			Input::Update();
		}
	}
}

void MultiPlayClient::RecvUpdate(int waitTime, RESPONSE_PLAYER &res) {
	// ファイルディスクリプタ
	FD tmp;
	memcpy(&tmp, &readfd_, sizeof(FD));
	Select(&tmp, nullptr, nullptr, 0, 1);

	Address serverAddr;
	int serverAddrLen = sizeof(serverAddr);



	std::cout << res_.time << std::endl;
	if (tmp.Contains(sockfd_)) {
		// 受信する
		int	buffLen = Recv(sockfd_, recvTmpBuff, MAX_BUFF, 0);
		// 終了
		if (buffLen <= 0) return;
		// データを取り込む
		recvBuff.Push(recvTmpBuff, buffLen);


		// レスポンスの解析
		res.ParseResponse(recvBuff);

		// モードがNONEではないなら
		if (res.mode != MULTI_MODE::NONE) {
			// 受信したモードと実行しているゲームモードが同じなら解析する
			//if (gameMode && res.mode == gameMode->GetMode()) gameMode->ParseResponse(recvBuff);

		}
		// レスポンスの保存
		res_ = res;

		std::cout << res_.time << std::endl;

		// 初期化
		recvBuff = nullptr;
	}
}

void MultiPlayClient::Update() {
	while (!isFinish) {
		RESPONSE_PLAYER res;

		if (GetAsyncKeyState(VK_ESCAPE)) {
			isFinish = true;
			break;
		}
		Graphical::Clear(Color(Color(1, 1, 1, 1) * 0.5f));
		Time::Update();
		RecvUpdate(1, res);
		PlayerUpdate(res);
		Graphical::Present();
		recvBuff = nullptr;
	}
}
