#include "multiplay.h"
#include "xinput.h"
#pragma comment(lib, "lib/lib.lib")



int MultiServer::Register(Address clientAddr, HEADER *header, Socket sockfd) {
	// 登録できる
	for (int i = 0; i < MAX_MEMBER; i++) {
		// 登録されていない
		if (clients_[i].header.id < 0) {

			// アドレスの設定
			clients_[i].clientAddr_ = clientAddr;

			// ソケットの設定
			clients_[i].sockfd_ = sockfd;

			// データがあるなら設定
			if (header) clients_[i].header = *header;

			// 最後にIDの設定
			clients_[i].header.id = i;

			// 送信
			clients_[i].header.command = HEADER::RESPONSE_LOGIN;
			SendTo(this->sockfd_, (char *)&clients_[i].header, sizeof(HEADER), 0, clientAddr);
			std::cout << "Res >> ID:" << i << " Login" << std::endl;

			return i;
		}
	}

	// 登録できない
	return -1;
}

void MultiServer::Unregister(int id) {
	// 範囲外
	if (0 <= id && id < MAX_MEMBER) return;

	// 登録している
	if (0 <= clients_[id].header.id) {
		// 解除
		clients_[id].header.command = HEADER::RESPONSE_LOGOUT;
		clients_[id].sockfd_.Close();

		// 送信
		SendTo(sockfd_, (char *)&clients_[id].header, sizeof(HEADER), 0, clients_[id].clientAddr_);
		std::cout << "Res >> ID:" << id << " Logout" << std::endl;

		// IDを削除
		clients_[id].header = HEADER();
	}
}

void MultiServer::PlayerUpdate(REQUEST_PLAYER req) {
	// キー入力の更新
	Input::SetState(0, req.curInput);
	Input::SetPreviousState(0, req.preInput);

	// ヘッダーの更新
	clients_[req.header.id].header = req.header;



	// プレイヤーの処理を行う
	players_[req.header.id].Update();
}

REQUEST_PLAYER MultiServer::RecvUpdate(void) {
	// リクエスト情報を書き出す
	REQUEST_PLAYER req;

	ParseRequestFromClient(recvBuff, &req);

	// リクエストの情報をもとに反映させる
	//PlayerUpdate(req);

	return req;
}

void MultiServer::SendUpdate(void) {
	// レスポンスの作成
	RESPONSE_PLAYER res[MAX_MEMBER];
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
	while (true) {
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
				int id = Register(clientAddr, pHeader, 0);
				// 送信
				SendTo(sockfd_, (char *)&clients_[id].header, sizeof(HEADER), 0, clients_[id].clientAddr_);
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
	}
}






int Client::Register() {
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
		SendTo(sockfd_, (char *)&header_, sizeof(HEADER), 0, serverAddr);

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

		std::cout << header_.id << "番目を解除しました。" << std::endl;

		sockfd_.Close();
	}

void Client::PlayerUpdate(RESPONSE_PLAYER res, RESPONSE_PLAYER reses) {

}

void Client::SendUpdate(void) {
	// リクエストの作成
	CreateRequestToServer(sendBuff);

	// 送信
	SendTo(sockfd_, sendBuff, sendBuff.Length(), 0, serverAddr);
}

void Client::RecvUpdate(int waitTime) {
	FD tmp;
	memcpy(&tmp, &readfd_, sizeof(FD));

	// 検知する
	const int MAX_BUFF = 1024;
	if (Select(&tmp, nullptr, nullptr, 0, waitTime)) {
		char buff[MAX_BUFF] = {};

		// 受信する
		int buffLen = Recv(sockfd_, (char *)buff, MAX_BUFF);
		recvBuff.Push(buff, buffLen);

		// レスポンスの解析
		RESPONSE_PLAYER reses[MAX_MEMBER] = {};
		ParseResponseFromServer(recvBuff, reses);

		// 更新
		for (RESPONSE_PLAYER &res : reses) {
			// 自分自身を更新
			if (res.header.id == header_.id) {
				header_ = res.header;
				if (header_.command == HEADER::RESPONSE_LOGOUT) return;
			}
			// 他人を更新
			else {
				// IDを0~MAX_MEMBER-1にする
				int id = header_.id;
				if (header_.id <= id) id--;

				// 更新
				if (res.header.command == HEADER::RESPONSE_LOGOUT) res.header = HEADER();
				players_[id].header = header_;
			}
		}
	}
}

void Client::Update() {
	RecvUpdate(10);
}

#include <thread>

Client client;

//void clientMultiplay() {
//	while (true) {
//		client.Send();
//		client.Update(100);
//		if (client.header.command == HEADER::RESPONSE_LOGOUT) return;
//	}
//}

//int main(void) {
//
//	int i = 0;
//	std::cin >> i;
//	system("cls");
//
//	if (i) {
//		client.Register();
//		std::thread thread__(clientMultiplay);
//
//		while (true) {
//			//system("cls");
//			if (_kbhit()) {
//				int key = _getch();
//				switch (key) {
//				case 'a': client.header.pos.x -= 1; break;
//				case 'd': client.header.pos.x += 1; break;
//				case 'w': client.header.pos.y -= 1; break;
//				case 's': client.header.pos.y += 1; break;
//				}
//				if ('l' == key) {
//					client.Unregister();
//					break;
//				}
//			}
//
//			//client.Send();
//			//client.Update(10);
//			//if (client.header.command == LOCAL_DATA::UNREGISTER_OK) return 0;
//
//
//			//Vector2Int pos = client.header.pos;
//			//Cursor(pos.x, pos.y);
//			//printf("*");
//			//for (int i = 0; i < MAX_MEMBER - 1; i++) {
//			//	pos = client.players_[i].pos;
//			//	Cursor(pos.x, pos.y);
//			//	printf("*");
//			//}
//		}
//
//		thread__.join();
//	}
//	else {
//		MultiServer server;
//		server.OpenTerminal();
//	}
//
//
//	return 0;
//}
