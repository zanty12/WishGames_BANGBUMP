#include "multiplay.h"
#include "xinput.h"
#include "thunder.h"
#include "fire.h"
#include "wind.h"
#include "dark.h"
#include "time.h"
#include <thread>
#pragma comment(lib, "lib/lib.lib")


MultiPlayServer::MultiPlayServer() {
	WSAData data;
	Startup(v2_2, data);

	mapmngr_ = new MapMngr("data/map/1.csv", this);
	gameMode = new MultiPlayIntermediateResultModeServerSide();//new MultiPlayAreaCaptureModeServerSide(mapmngr_);
}

int MultiPlayServer::Register(Address clientAddr, HEADER &header, Socket sockfd) {
	// ロック解除待機
	while (isListLock);

	// ロック
	isListLock = true;

	// プレイヤー作成
	Vector2 pos = Vector2(200, 200);
	float rot = 0.0f;
	int texNo = 0;
	Vector2 vel = Vector2::Zero;
	Player *player = new Player(pos, rot, texNo, vel, mapmngr_);
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
	isListLock = false;

	return maxID - 1;
}

void MultiPlayServer::Unregister(int id) {
	// ロック解除待機
	while (isListLock);

	// ロック
	isListLock = true;





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
	isListLock = false;
}

void MultiPlayServer::AllUnregister(void) {
	while (clients_.size())
	{
		Unregister(clients_.begin()->header.id);
	}
}

void MultiPlayServer::PlayerUpdate(void) {
	// プレイヤーの更新
	for (auto client : clients_) {
		// 入力の更新
		Input::SetState(0, client.currentInput);
		Input::SetPreviousState(0, client.previousInput);
		
		// プレイヤーの更新
		client.player_->Update();
	}

	// コリジョンの更新
	if (coll_mngr_) coll_mngr_->Update();

	// ゲームモードの更新
	if (gameMode) gameMode->Update(clients_);
}

void MultiPlayServer::RecvUpdate(void) {
	// リクエスト情報を書き出す
	REQUEST_PLAYER req;
	req.ParseRequest(recvBuff);



	// プレイヤーの検索
	auto iterator = find(req.input.id);

	// 検索不一致
	if (iterator == clients_.end()) return;

	// 入力情報を設定
	iterator->currentInput = req.input.curInput;
	iterator->previousInput = req.input.preInput;
}

void MultiPlayServer::SendUpdate(void) {
	DWORD startTime, currentTime, onceFrameTime;
	startTime = currentTime = timeGetTime();
	onceFrameTime = 1000 / 60;

	while (true) {
		if (!isListLock) {
			// ロック
			isListLock = true;

			currentTime = timeGetTime();

			if (currentTime - startTime > onceFrameTime) {
				startTime = currentTime;

				// レスポンスの作成
				RESPONSE_PLAYER res;

				// 制限時間の登録
				res.time= gameMode->GetTime();
				res.maxTime = gameMode->GetMaxTime();

				// クライアント情報の登録
				for (auto &client : clients_) {
					res.clients.push_back({ client.header.id , client.player_->GetPos(), 0, 0 });
				}

				// クライアント全員に送信する
				for (auto &client : clients_) {
					// 登録されていないならスキップ
					if (client.header.id < 0) continue;

					// 宛先の登録とレスポンス内容の結合
					res.CreateResponse(sendBuff, client.header.id);

					// ゲームモードのレスポンス内容の結合
					gameMode->CreateResponse(sendBuff);

					// 送信
					SendTo(sockfd_, sendBuff, sendBuff.Length(), 0, client.clientAddr_);
				}
			}
			// ロック解除
			isListLock = false;
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



	const int MAX_BUFF = 1024;
	MSG msg;
	while (true) {
		// メッセージ
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
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
					//SendTo(sockfd_, (char *)&clients_[id].header, sizeof(HEADER), 0, clients_[id].clientAddr_);
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

		{
			currentTime = timeGetTime();
			// 更新
			if (currentTime - startTime > onceFrameTime) {
				startTime = currentTime;

				PlayerUpdate();
			}
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

	mapmngr_ = new MapMngr("data/map/1.csv", this);
	gameMode = new MultiPlayIntermediateResultModeClientSide();//new MultiPlayAreaCaptureModeClientSide(mapmngr_);

	// スレッドを立てる
	sendUpdateFunc = std::thread(&MultiPlayClient::SendUpdate, this);
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

	while (true) {
		HEADER data;
		Recv(sockfd_, (char *)&data, sizeof(HEADER), 0);

		// ㇿグラウト成功なら
		if (data.command == HEADER::RESPONSE_LOGOUT &&
			data.id == header.id) {
			std::cout << "Res << ID:" << id << " Logout" << std::endl;
			break;
		}
	}

	std::cout << id << "を解除しました。" << std::endl;

	sockfd_.Close();
}

void MultiPlayClient::PlayerUpdate(RESPONSE_PLAYER &res) {
	//playerAnim.Update(res);
	//playerAnim.Draw();
	//if (res.clients.size()) std::cout << res.clients.begin()->position.x << ", " << res.clients.begin()->position.y << std::endl;

	for (auto &client : res.clients) {
		//std::cout << client.position.x << ", " << client.position.y << std::endl;
		anim.SetPos(client.position);
		anim.Draw();
	}

	if (gameMode) gameMode->Draw(res);
	renderer_->Draw();
	//Camera camera = Camera();
	//mapMngr.Draw();
}

void MultiPlayClient::SendUpdate(void) {
	DWORD startTime, currentTime, onceFrameTime;
	startTime = currentTime = timeGetTime();
	onceFrameTime = 1000 / 60;

	while (true) {
		currentTime = timeGetTime();

		if (currentTime - startTime > onceFrameTime) {
			startTime = currentTime;

			// 入力更新
			Input::Update();

			// リクエストの作成
			REQUEST_PLAYER req;
			req.input = { id, Input::GetState(0), Input::GetPreviousState(0) };
			req.CreateRequest(sendBuff, id);

			//std::cout << "0000 : : " << Input::GetStickLeft(0).x << ", " << Input::GetStickLeft(0).y << std::endl;

			// 送信
			SendTo(sockfd_, sendBuff, sendBuff.Length(), 0, serverAddr);
			sendBuff = nullptr;
		}
	}
}

void MultiPlayClient::RecvUpdate(int waitTime, RESPONSE_PLAYER &res) {
	// ファイルディスクリプタ
	FD tmp;
	const int MAX_BUFF = 1024;
	memcpy(&tmp, &readfd_, sizeof(FD));

	// 検知する
	if (Select(&tmp, nullptr, nullptr, 0, waitTime)) {
		char buff[MAX_BUFF] = {};

		// 受信する
		int buffLen = Recv(sockfd_, (char *)buff, MAX_BUFF);
		// 失敗なら終了
		if (buffLen <= 0) return;
		recvBuff.Push(buff, buffLen);



		// レスポンスの解析
		res.ParseResponse(recvBuff);
		if (gameMode) gameMode->ParseResponse(recvBuff);


		// 初期化
		recvBuff = nullptr;
	}
}

void MultiPlayClient::Update() {
	RESPONSE_PLAYER res;

	RecvUpdate(1, res);
	PlayerUpdate(res);
	recvBuff = nullptr;
}
