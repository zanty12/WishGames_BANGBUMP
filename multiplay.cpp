#include "multiplay.h"
#pragma comment(lib, "lib/lib.lib")




int MultiServer::Register(Address clientAddr, LOCAL_DATA *localData, Socket sockfd) {
		// 登録できる
		for (int i = 0; i < MAX_MEMBER; i++) {
			// 登録されていない
			if (players_[i].localData.id < 0) {

				// アドレスの設定
				players_[i].clientAddr_ = clientAddr;

				// ソケットの設定
				players_[i].sockfd_ = sockfd;

				// データがあるなら設定
				if (localData) players_[i].localData = *localData;

				// 最後にIDの設定
				players_[i].localData.id = i;

				// 送信
				players_[i].localData.command = LOCAL_DATA::RESPONSE_LOGIN;
				SendTo(this->sockfd_, (char *)&players_[i].localData, sizeof(LOCAL_DATA), 0, clientAddr);
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
		if (0 <= players_[id].localData.id) {
			// 解除
			players_[id].localData.command = LOCAL_DATA::RESPONSE_LOGOUT;
			players_[id].sockfd_.Close();

			// 送信
			SendTo(sockfd_, (char *)&players_[id].localData, sizeof(LOCAL_DATA), 0, players_[id].clientAddr_);
			std::cout << "Res >> ID:" << id << " Logout" << std::endl;

			// IDを削除
			players_[id].localData = LOCAL_DATA();
		}
	}

void MultiServer::SendUpdate(LOCAL_DATA *pLocalData) {
	if (pLocalData == nullptr) return;

	// 更新
	players_[pLocalData->id].localData = *pLocalData;

	for (int i = 0; i < MAX_MEMBER; i++) {
		// 登録されていないならスキップ
		// 更新相手が自分ならスキップ
		if (players_[i].localData.id < 0 ||
			players_[i].localData.id == pLocalData->id) continue;

		players_[pLocalData->id].localData.command = LOCAL_DATA::RESPONSE_UPDATE;
		SendTo(sockfd_, (char *)&players_[pLocalData->id].localData, sizeof(LOCAL_DATA), 0, players_[i].clientAddr_);
	}
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
				LOCAL_DATA *pLocalData = (LOCAL_DATA *)buff;

				switch (pLocalData->command)
				{
				case LOCAL_DATA::COMMAND::REQUEST_LOGIN: {
					// 登録
					std::cout << "Req << ID:*" << " Login" << std::endl;
					int id = Register(clientAddr, pLocalData, 0);
					// 送信
					SendTo(sockfd_, (char *)&players_[id].localData, sizeof(LOCAL_DATA), 0, players_[id].clientAddr_);
					std::cout << id << " : 登録しました" << std::endl;
					break;
				};
				case LOCAL_DATA::COMMAND::REQUEST_LOGOUT: {
					// 解除
					std::cout << "Req << ID:" << pLocalData->id << " Logout" << std::endl;
					Unregister(pLocalData->id);
					std::cout << pLocalData->id << " : 解除しました" << std::endl;
					break;
				}
				case LOCAL_DATA::COMMAND::REQUEST_UPDATE: {
					// 更新
					SendUpdate(pLocalData);
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
		localData.command = LOCAL_DATA::COMMAND::REQUEST_LOGIN;

		// 操作
		readfd_.Add(sockfd_);

		// 送信
		std::cout << "Req >> ID:*" << " Login" << std::endl;
		SendTo(sockfd_, (char *)&localData, sizeof(LOCAL_DATA), 0, serverAddr);

		// 受信
		while (true) {
			Recv(sockfd_, (char *)&localData, sizeof(LOCAL_DATA), 0);
			if (localData.command = LOCAL_DATA::RESPONSE_LOGIN) break;
		}
		std::cout << "Res << ID:" << localData.id << " Login" << std::endl;
		std::cout << localData.id << "番目に登録しました。" << std::endl;

		return localData.id;
	}

void Client::Unregister() {
		// コマンド設定
		localData.command = LOCAL_DATA::REQUEST_LOGOUT;

		// 送信
		std::cout << "Req << ID:" << localData.id << " Logout" << std::endl;
		SendTo(sockfd_, (char *)&localData, sizeof(LOCAL_DATA), 0, serverAddr);

		while (true) {
			LOCAL_DATA data;
			Recv(sockfd_, (char *)&data, sizeof(LOCAL_DATA), 0);
			if (data.command == LOCAL_DATA::RESPONSE_LOGOUT &&
				data.id == localData.id) {
				std::cout << "Res << ID:" << localData.id << " Logout" << std::endl;
				localData = LOCAL_DATA();
				break;
			}
		}

		std::cout << localData.id << "番目を解除しました。" << std::endl;

		sockfd_.Close();
	}

void Client::Send(void) {
		// コマンド設定
		localData.command = LOCAL_DATA::REQUEST_UPDATE;

		// 送信
		SendTo(sockfd_, (char *)&localData, sizeof(LOCAL_DATA), 0, serverAddr);
	}

void Client::Update(int waitTime) {
		FD tmp;
		memcpy(&tmp, &readfd_, sizeof(FD));
		// 検知する
		if (Select(&tmp, nullptr, nullptr, 0, waitTime)) {
			LOCAL_DATA localData;

			// 受信する
			Recv(sockfd_, (char *)&localData, sizeof(LOCAL_DATA));

			// 自分自身を更新
			if (localData.id == this->localData.id) {
				this->localData = localData;
				if (this->localData.command == LOCAL_DATA::RESPONSE_LOGOUT) return;
			}
			// 他人を更新
			else {
				// IDを0~MAX_MEMBER-1にする
				int id = localData.id;
				if (this->localData.id <= id) id--;

				// 更新
				if (localData.command == LOCAL_DATA::RESPONSE_LOGOUT) localData = LOCAL_DATA();
				players_[id] = localData;
			}
		}
	}


#include <thread>

Client client;

void clientMultiplay() {
	while (true) {
		client.Send();
		client.Update(100);
		if (client.localData.command == LOCAL_DATA::RESPONSE_LOGOUT) return;
	}
}

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
//				case 'a': client.localData.pos.x -= 1; break;
//				case 'd': client.localData.pos.x += 1; break;
//				case 'w': client.localData.pos.y -= 1; break;
//				case 's': client.localData.pos.y += 1; break;
//				}
//				if ('l' == key) {
//					client.Unregister();
//					break;
//				}
//			}
//
//			//client.Send();
//			//client.Update(10);
//			//if (client.localData.command == LOCAL_DATA::UNREGISTER_OK) return 0;
//
//
//			//Vector2Int pos = client.localData.pos;
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
