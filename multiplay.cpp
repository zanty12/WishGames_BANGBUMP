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
	auto iterator = find(req.input.id);

	// 検索不一致
	if (iterator == clients_.end()) return;

	// キー入力の更新
	Input::SetState(0, req.input.curInput);
	Input::SetPreviousState(0, req.input.preInput);




	// プレイヤーの処理を行う
	iterator->player_->Update();
	std::cout << clients_.size() << std::endl;
	//std::cout << iterator->player_->GetPos().x << ", " << iterator->player_->GetPos().y << std::endl;
}

REQUEST_PLAYER MultiServer::RecvUpdate(void) {
	// リクエスト情報を書き出す
	REQUEST_PLAYER req;
	req.ParseRequest(recvBuff);

	//ParseRequestFromClient(recvBuff, req);

	return req;
}

void MultiServer::SendUpdate(void) {
	// レスポンスの作成
	RESPONSE_PLAYER res;
	//CreateResponseToClient(sendContentBuff);

	// クライアント情報の登録
	for (auto &client : clients_) {
		res.clients.push_back({ client.header.id , client.player_->GetPos() });
	}

	// クライアント全員に送信する
	for (auto &client : clients_) {
		// 登録されていないならスキップ
		if (client.header.id < 0) continue;

		// 宛先の登録とレスポンス内容の結合
		res.CreateResponse(sendBuff, client.header.id);

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

void Client::Unregister() {
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

void Client::PlayerUpdate(RESPONSE_PLAYER &res) {
	//playerAnim.Update(res);
	//playerAnim.Draw();

	for (auto &client : res.clients) {
		std::cout << client.position.x << ", " << client.position.y << std::endl;
		playerAnim.Update(client);
		playerAnim.Draw();
	}
}

void Client::SendUpdate(void) {
	// リクエストの作成
	REQUEST_PLAYER req;
	req.input = { id, Input::GetState(0), Input::GetPreviousState(0) };
	req.CreateRequest(sendBuff, id);

	// 送信
	SendTo(sockfd_, sendBuff, sendBuff.Length(), 0, serverAddr);
}

void Client::RecvUpdate(int waitTime, RESPONSE_PLAYER &res) {
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
		res.ParseResponse(recvBuff);
	}
}

void Client::Update() {
	RESPONSE_PLAYER res;

	RecvUpdate(10, res);
	PlayerUpdate(res);
	SendUpdate();
	recvBuff = nullptr;
	sendBuff = nullptr;
}
