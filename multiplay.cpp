#include "multiplay.h"
#include "xinput.h"
#include "thunder.h"
#include "fire.h"
#include "wind.h"
#include "dark.h"
#pragma comment(lib, "lib/lib.lib")



int MultiServer::Register(Address clientAddr, HEADER &header, Socket sockfd) {
	Vector2 pos = Vector2::Zero;
	float rot = 0.0f;
	int texNo = 0;
	Vector2 vel = Vector2::Zero;
	Player *player = new Player(pos, rot, texNo, vel, &mapMngr);
	player->SetAttribute(new Fire(player));
	player->SetAttackAttribute(new Fire(player));

	// ヘッダーの更新
	header.command = HEADER::RESPONSE_LOGIN;
	header.id = maxID++;

	// クライアントデータの作成
	CLIENT_DATA clientData = {
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

	return maxID - 1;
}

void MultiServer::Unregister(int id) {

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
}

void MultiServer::AllUnregister(void) {
	while (clients_.size())
	{
		Unregister(clients_.begin()->header.id);
	}
}

void MultiServer::PlayerUpdate(REQUEST_PLAYER req) {

	// プレイヤーの検索
	auto iterator = find(req.header.id);

	// 検索不一致
	if (iterator == clients_.end()) return;

	// キー入力の更新
	Input::SetState(0, req.curInput);
	Input::SetPreviousState(0, req.preInput);

	// ヘッダーの更新
	iterator->header = req.header;



	// プレイヤーの処理を行う
	iterator->player_->Update();
	std::cout << clients_.size() << std::endl;
	//std::cout << iterator->player_->GetPos().x << ", " << iterator->player_->GetPos().y << std::endl;
}

REQUEST_PLAYER MultiServer::RecvUpdate(void) {
	// リクエスト情報を書き出す
	REQUEST_PLAYER req;

	ParseRequestFromClient(recvBuff, req);

	return req;
}

void MultiServer::SendUpdate(void) {
	// レスポンスの作成
	CreateResponseToClient(sendContentBuff);


	for (auto &client : clients_) {
		// 登録されていないならスキップ
		if (client.header.id < 0) continue;

		client.header.command = HEADER::RESPONSE_UPDATE;

		// 宛先の登録とレスポンス内容の結合
		sendBuff = client.header;
		sendBuff << sendContentBuff;

		// 送信
		SendTo(sockfd_, sendBuff, sendBuff.Length(), 0, client.clientAddr_);
	}
}

void MultiServer::Update() {
	REQUEST_PLAYER req = RecvUpdate();
	PlayerUpdate(req);
	SendUpdate();
}

void MultiServer::OpenTerminal(void) {
	// ソケット作成
	sockfd_ = Socket(AddressFamily::IPV4, Type::UDP, 0);
	// バインド用アドレス作成
	Address addr = Address(AddressFamily::IPV4, INADDR_ANY, PORT);
	// バインド
	sockfd_.Bind(addr);

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
				Update();
				break;
			}
			}
		}

		recvBuff = nullptr;
		sendBuff = nullptr;
	}
}


int Client::Register() {
	HEADER &header_ = player.header;

	// ソケット作成
	sockfd_ = Socket(AddressFamily::IPV4, Type::UDP, 0);

	// アドレス作成
	serverAddr = Address(AddressFamily::IPV4, SERVER_ADDRESS, PORT);

	// コマンド設定
	header_.command = HEADER::COMMAND::REQUEST_LOGIN;

	// 操作
	readfd_.Add(sockfd_);

	// 送信
	std::cout << "Req >> ID:*" << " Login" << std::endl;
	SendTo(sockfd_, (char *)&player.header, sizeof(HEADER), 0, serverAddr);

	// 受信
	while (true) {
		Recv(sockfd_, (char *)&header_, sizeof(HEADER), 0);
		if (header_.command = HEADER::RESPONSE_LOGIN) break;
	}
	std::cout << "Res << ID:" << header_.id << " Login" << std::endl;
	std::cout << header_.id << "番目に登録しました。" << std::endl;
	return header_.id;
}

void Client::Unregister() {
	HEADER &header_ = player.header;

	// コマンド設定
	header_.command = HEADER::REQUEST_LOGOUT;

	// 送信
	std::cout << "Req << ID:" << header_.id << " Logout" << std::endl;
	SendTo(sockfd_, (char *)&header_, sizeof(HEADER), 0, serverAddr);

	while (true) {
		HEADER data;
		Recv(sockfd_, (char *)&data, sizeof(HEADER), 0);
		if (data.command == HEADER::RESPONSE_LOGOUT &&
			data.id == header_.id) {
			std::cout << "Res << ID:" << header_.id << " Logout" << std::endl;
			header_ = HEADER();
			break;
		}
	}

	std::cout << header_.id << "を解除しました。" << std::endl;

	sockfd_.Close();
}

void Client::PlayerUpdate(RESPONSE_PLAYER &res, std::list<RESPONSE_PLAYER> &reses) {
	std::cout << res.position.x << ", " << res.position.y << std::endl;
	playerAnim.Update(res);
	playerAnim.Draw();

	for (auto &res : reses) {
		playerAnim.Update(res);
		playerAnim.Draw();
	}
}

void Client::SendUpdate(void) {
	// リクエストの作成
	CreateRequestToServer(sendBuff);

	// 送信
	SendTo(sockfd_, sendBuff, sendBuff.Length(), 0, serverAddr);
}

void Client::RecvUpdate(int waitTime, RESPONSE_PLAYER &res, std::list<RESPONSE_PLAYER> &reses) {
	FD tmp;
	memcpy(&tmp, &readfd_, sizeof(FD));

	// 検知する
	const int MAX_BUFF = 1024;
	if (Select(&tmp, nullptr, nullptr, 0, waitTime)) {
		char buff[MAX_BUFF] = {};

		// 受信する
		int buffLen = Recv(sockfd_, (char *)buff, MAX_BUFF);
		// 失敗なら終了
		if (buffLen <= 0) return;
		recvBuff.Push(buff, buffLen);

		// レスポンスの解析
		std::list<RESPONSE_PLAYER> clients;
		ParseResponseFromServer(recvBuff, clients);

		// 更新
		for (RESPONSE_PLAYER &r : clients) {
			// 自分自身を更新
			if (r.header.id == player.header.id) {
				player.header = r.header;
				player.position = r.position;

				res = player;
				if (player.header.command == HEADER::RESPONSE_LOGOUT) return;
			}
			// 他人を更新
			else {
				// 更新
				if (r.header.command == HEADER::RESPONSE_LOGOUT) r.header = HEADER();
				reses.push_back(r);
			}
		}
	}
}

void Client::Update() {
	RESPONSE_PLAYER res;
	std::list<RESPONSE_PLAYER> reses;

	RecvUpdate(10, res, reses);
	PlayerUpdate(res, reses);
	SendUpdate();
	recvBuff = nullptr;
	sendBuff = nullptr;
}
