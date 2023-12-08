#include "multiplay.h"
#pragma comment(lib, "lib/lib.lib")




int MultiServer::Register(Address clientAddr, LOCAL_DATA *localData, Socket sockfd) {
		// �o�^�ł���
		for (int i = 0; i < MAX_MEMBER; i++) {
			// �o�^����Ă��Ȃ�
			if (players_[i].localData.id < 0) {

				// �A�h���X�̐ݒ�
				players_[i].clientAddr_ = clientAddr;

				// �\�P�b�g�̐ݒ�
				players_[i].sockfd_ = sockfd;

				// �f�[�^������Ȃ�ݒ�
				if (localData) players_[i].localData = *localData;

				// �Ō��ID�̐ݒ�
				players_[i].localData.id = i;

				// ���M
				players_[i].localData.command = LOCAL_DATA::RESPONSE_LOGIN;
				SendTo(this->sockfd_, (char *)&players_[i].localData, sizeof(LOCAL_DATA), 0, clientAddr);
				std::cout << "Res >> ID:" << i << " Login" << std::endl;

				return i;
			}
		}

		// �o�^�ł��Ȃ�
		return -1;
	}

void MultiServer::Unregister(int id) {
		// �͈͊O
		if (0 <= id && id < MAX_MEMBER) return;

		// �o�^���Ă���
		if (0 <= players_[id].localData.id) {
			// ����
			players_[id].localData.command = LOCAL_DATA::RESPONSE_LOGOUT;
			players_[id].sockfd_.Close();

			// ���M
			SendTo(sockfd_, (char *)&players_[id].localData, sizeof(LOCAL_DATA), 0, players_[id].clientAddr_);
			std::cout << "Res >> ID:" << id << " Logout" << std::endl;

			// ID���폜
			players_[id].localData = LOCAL_DATA();
		}
	}

void MultiServer::SendUpdate(LOCAL_DATA *pLocalData) {
	if (pLocalData == nullptr) return;

	// �X�V
	players_[pLocalData->id].localData = *pLocalData;

	for (int i = 0; i < MAX_MEMBER; i++) {
		// �o�^����Ă��Ȃ��Ȃ�X�L�b�v
		// �X�V���肪�����Ȃ�X�L�b�v
		if (players_[i].localData.id < 0 ||
			players_[i].localData.id == pLocalData->id) continue;

		players_[pLocalData->id].localData.command = LOCAL_DATA::RESPONSE_UPDATE;
		SendTo(sockfd_, (char *)&players_[pLocalData->id].localData, sizeof(LOCAL_DATA), 0, players_[i].clientAddr_);
	}
}

void MultiServer::OpenTerminal(void) {
		// �\�P�b�g�쐬
		sockfd_ = Socket(AddressFamily::IPV4, Type::UDP, 0);
		// �o�C���h�p�A�h���X�쐬
		Address addr = Address(AddressFamily::IPV4, INADDR_ANY, PORT);
		// �o�C���h
		sockfd_.Bind(addr);

		const int MAX_BUFF = 1024;
		while (true) {
			Address clientAddr;
			int clientAddrLen = sizeof(clientAddr);
			char buff[MAX_BUFF] = {};

			// ��M
			int buffLen = RecvFrom(sockfd_, buff, MAX_BUFF, 0, &clientAddr, &clientAddrLen);

			if (0 <= buffLen) {
				LOCAL_DATA *pLocalData = (LOCAL_DATA *)buff;

				switch (pLocalData->command)
				{
				case LOCAL_DATA::COMMAND::REQUEST_LOGIN: {
					// �o�^
					std::cout << "Req << ID:*" << " Login" << std::endl;
					int id = Register(clientAddr, pLocalData, 0);
					// ���M
					SendTo(sockfd_, (char *)&players_[id].localData, sizeof(LOCAL_DATA), 0, players_[id].clientAddr_);
					std::cout << id << " : �o�^���܂���" << std::endl;
					break;
				};
				case LOCAL_DATA::COMMAND::REQUEST_LOGOUT: {
					// ����
					std::cout << "Req << ID:" << pLocalData->id << " Logout" << std::endl;
					Unregister(pLocalData->id);
					std::cout << pLocalData->id << " : �������܂���" << std::endl;
					break;
				}
				case LOCAL_DATA::COMMAND::REQUEST_UPDATE: {
					// �X�V
					SendUpdate(pLocalData);
					break;
				}
				}
			}
		}
	}






int Client::Register() {
		// �\�P�b�g�쐬
		sockfd_ = Socket(AddressFamily::IPV4, Type::UDP, 0);

		// �A�h���X�쐬
		serverAddr = Address(AddressFamily::IPV4, SERVER_ADDRESS, PORT);

		// �R�}���h�ݒ�
		localData.command = LOCAL_DATA::COMMAND::REQUEST_LOGIN;

		// ����
		readfd_.Add(sockfd_);

		// ���M
		std::cout << "Req >> ID:*" << " Login" << std::endl;
		SendTo(sockfd_, (char *)&localData, sizeof(LOCAL_DATA), 0, serverAddr);

		// ��M
		while (true) {
			Recv(sockfd_, (char *)&localData, sizeof(LOCAL_DATA), 0);
			if (localData.command = LOCAL_DATA::RESPONSE_LOGIN) break;
		}
		std::cout << "Res << ID:" << localData.id << " Login" << std::endl;
		std::cout << localData.id << "�Ԗڂɓo�^���܂����B" << std::endl;

		return localData.id;
	}

void Client::Unregister() {
		// �R�}���h�ݒ�
		localData.command = LOCAL_DATA::REQUEST_LOGOUT;

		// ���M
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

		std::cout << localData.id << "�Ԗڂ��������܂����B" << std::endl;

		sockfd_.Close();
	}

void Client::Send(void) {
		// �R�}���h�ݒ�
		localData.command = LOCAL_DATA::REQUEST_UPDATE;

		// ���M
		SendTo(sockfd_, (char *)&localData, sizeof(LOCAL_DATA), 0, serverAddr);
	}

void Client::Update(int waitTime) {
		FD tmp;
		memcpy(&tmp, &readfd_, sizeof(FD));
		// ���m����
		if (Select(&tmp, nullptr, nullptr, 0, waitTime)) {
			LOCAL_DATA localData;

			// ��M����
			Recv(sockfd_, (char *)&localData, sizeof(LOCAL_DATA));

			// �������g���X�V
			if (localData.id == this->localData.id) {
				this->localData = localData;
				if (this->localData.command == LOCAL_DATA::RESPONSE_LOGOUT) return;
			}
			// ���l���X�V
			else {
				// ID��0~MAX_MEMBER-1�ɂ���
				int id = localData.id;
				if (this->localData.id <= id) id--;

				// �X�V
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
