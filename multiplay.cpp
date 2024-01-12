#include "multiplay.h"
#include "xinput.h"
#include "thunder.h"
#include "fire.h"
#include "wind.h"
#include "dark.h"
#include "time.h"
#include "enemy1.h"
#include <windows.h>
#include <thread>
#pragma comment(lib, "lib/lib.lib")

//#define DEBUG_INPUT
#define DEBUG_LOCKED
//#define DEBUG_SENDLEN

MultiPlayServer::MultiPlayServer() {
	WSAData data;
	Startup(v2_2, data);

	gameMode = new MultiPlayFlowServerSide(this);
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
	Player *player = new Player(pos, rot, vel, mapmngr_);
	player->SetAttribute(new Fire(player));
	player->SetAttackAttribute(new Fire(player));

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
	clients_.push_back(clientData);


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





	// プレイヤーの検索
	auto iterator = find(id);

	// 検索不一致
	if (iterator == clients_.end()) return;

	// 解除
	iterator->header.command = HEADER::RESPONSE_LOGOUT;
	iterator->sockfd_.Close();

	// 送信
	SendTo(sockfd_, (char *)&iterator->header, sizeof(HEADER), 0, iterator->clientAddr_);
	std::cout << "Res >> ID:" << " Logout" << std::endl;

	// 削除
	delete iterator->player_;
	clients_.erase(iterator);



	// ロック解除
#ifdef DEBUG_LOCKED
	std::cout << " - UNREGISTER UNLOCK" << std::endl;
#endif
	lock_.Unlock();
}

void MultiPlayServer::AllUnregister(void) {
	while (clients_.size())
	{
		Unregister(clients_.begin()->header.id);
	}
}

void MultiPlayServer::PlayerUpdate(void) {
	// ロック
	lock_.Lock();
#ifdef DEBUG_LOCKED
	std::cout << "UPD LOCK";
#endif

	// プレイヤーの更新
	for (auto client : clients_) {
		// 入力の更新
		Input::SetState(0, client.currentInput);
		Input::SetPreviousState(0, client.previousInput);
		
		// プレイヤーの更新
		client.player_->Update();
	}


	
	// コリジョンの更新
	if (coll_mngr_) {
		coll_mngr_->Update();
	}

	// ゲームモードの更新
	if (gameMode) gameMode->Update(clients_);

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
	auto iterator = find(req.input.id);

	// 検索したなら
	if (iterator != clients_.end()) {
		// 入力情報を設定
		iterator->currentInput = req.input.curInput;
		iterator->previousInput = req.input.preInput;

#ifdef DEBUG_INPUT
		Input::SetState(1, iterator->currentInput);
		Input::SetPreviousState(1, iterator->previousInput);
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

			// レスポンス情報の登録
			res.mode = gameMode->GetMode();
			res.maxTime = gameMode->GetMaxTime();
			res.time = gameMode->GetTime();

			// クライアント情報の登録
			for (auto &client : clients_) {
				res.clients.push_back({ client.header.id, client.moveAttribute, client.actionAttribute, client.player_->GetPos(), 0, 0 });
			}

			// オブジェクト情報の登録
			for (auto &skillorb : orb_mngr_->GetSkillOrbs()) {
				if (skillorb->GetDiscard()) continue;
				int id = std::atoi(skillorb->GetID().c_str());
				Vector2 position = skillorb->GetPos();
				float rotation = skillorb->GetRot();
				Vector2 scale = skillorb->GetScl();
				res.objects.push_back({
					id,										// ID
					OBJECT_DATA_CLIENT_SIDE::SKILL_POINT,	// tag
					OBJECT_DATA_CLIENT_SIDE::NONE,			// animation
					position,								// pos
					rotation,								// rot
					scale									// scl
					}
				);
			}

			// クライアント全員に送信する
			for (auto &client : clients_) {
				// 登録されていないならスキップ
				if (client.header.id < 0) continue;

				// 宛先の登録とレスポンス内容の結合
				res.CreateResponse(sendBuff, client.header.id);

				// ゲームモードのレスポンス内容の結合
				gameMode->CreateResponse(sendBuff);

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

void MultiPlayServer::Update() {
	//// 衝突判定
	//coll_mngr_->Update();
	//// プレイヤーアップデート
	//PlayerUpdate(req);
	//// ゲームモードアップデート
	//gameMode->Update(clients_);
	
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











MultiPlayClient::MultiPlayClient() : texNo(LoadTexture("data/texture/player.png")), anim(Animator(&playerObject, 2, true, 1, 1, 1)) {
	WSAData data;
	Startup(v2_2, data);

	gameMode = new MultiPlayFlowClientSide(this);
	multiRenderer_ = new MultiRenderer();

	// スレッドを立てる
	sendUpdateFunc = std::thread(&MultiPlayClient::SendUpdate, this);
	recvUpdateFunc = std::thread(&MultiPlayClient::Update, this);

	// 受信用領域を確保する
	recvTmpBuff = new char[MAX_BUFF];
}

int MultiPlayClient::Register() {
	HEADER header;

	// ソケット作成
	sockfd_ = Socket(AddressFamily::IPV4, Type::UDP, 0);

	// アドレス作成
	serverAddr = Address(AddressFamily::IPV4, SERVER_ADDRESS, PORT);

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

	//while (true) {
	//	HEADER data;
	//	Recv(sockfd_, (char *)&data, sizeof(HEADER), 0);

	//	// ㇿグラウト成功なら
	//	if (data.command == HEADER::RESPONSE_LOGOUT &&
	//		data.id == header.id) {
	//		std::cout << "Res << ID:" << id << " Logout" << std::endl;
	//		break;
	//	}
	//}

	std::cout << id << "を解除しました。" << std::endl;

	sockfd_.Close();
}

void MultiPlayClient::PlayerUpdate(RESPONSE_PLAYER &res) {
	// モードがNONEなら終了
	if (res_.mode == MULTI_MODE::NONE || res.clients.size() == 0) {
		return;
	}



	for (auto &client : res_.clients) {
		anim.SetPos(client.position);
		anim.Draw();
	}

	if (gameMode) gameMode->Draw(res_);
	renderer_->CheckDiscard();
	coll_mngr_->CheckDiscard();
	multiRenderer_->Draw(res_);
	renderer_->Draw(Vector2(0.0f, res.clients.begin()->position.y));
	
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
			req.input = { id, Input::GetState(0), Input::GetPreviousState(0) };
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
			if (gameMode && res.mode == gameMode->GetMode()) gameMode->ParseResponse(recvBuff);

			// レスポンスの保存
			res_ = res;
		}

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
		RecvUpdate(1, res);
		PlayerUpdate(res);
		Graphical::Present();
		recvBuff = nullptr;
	}
}
