#include "multiplay.h"
#include "xinput.h"
#pragma comment(lib, "lib/lib.lib")



int MultiServer::Register(Address clientAddr, HEADER *header, Socket sockfd) {
	// �o�^�ł���
	for (int i = 0; i < MAX_MEMBER; i++) {
		// �o�^����Ă��Ȃ�
		if (clients_[i].header.id < 0) {

			// �A�h���X�̐ݒ�
			clients_[i].clientAddr_ = clientAddr;

			// �\�P�b�g�̐ݒ�
			clients_[i].sockfd_ = sockfd;

			// �f�[�^������Ȃ�ݒ�
			if (header) clients_[i].header = *header;

			// �Ō��ID�̐ݒ�
			clients_[i].header.id = i;

			// ���M
			clients_[i].header.command = HEADER::RESPONSE_LOGIN;
			SendTo(this->sockfd_, (char *)&clients_[i].header, sizeof(HEADER), 0, clientAddr);
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
	if (0 <= clients_[id].header.id) {
		// ����
		clients_[id].header.command = HEADER::RESPONSE_LOGOUT;
		clients_[id].sockfd_.Close();

		// ���M
		SendTo(sockfd_, (char *)&clients_[id].header, sizeof(HEADER), 0, clients_[id].clientAddr_);
		std::cout << "Res >> ID:" << id << " Logout" << std::endl;

		// ID���폜
		clients_[id].header = HEADER();
	}
}

void MultiServer::PlayerUpdate(REQUEST_PLAYER req) {
	// �L�[���͂̍X�V
	Input::SetState(0, req.curInput);
	Input::SetPreviousState(0, req.preInput);

	// �w�b�_�[�̍X�V
	clients_[req.header.id].header = req.header;



	// �v���C���[�̏������s��
	players_[req.header.id].Update();
}

REQUEST_PLAYER MultiServer::RecvUpdate(void) {
	// ���N�G�X�g���������o��
	REQUEST_PLAYER req;

	ParseRequestFromClient(recvBuff, &req);

	// ���N�G�X�g�̏������Ƃɔ��f������
	//PlayerUpdate(req);

	return req;
}

void MultiServer::SendUpdate(void) {
	// ���X�|���X�̍쐬
	RESPONSE_PLAYER res[MAX_MEMBER];
	CreateResponseToClient(sendContentBuff);


	for (auto &client : clients_) {
		// �o�^����Ă��Ȃ��Ȃ�X�L�b�v
		if (client.header.id < 0) continue;

		client.header.command = HEADER::RESPONSE_UPDATE;

		// ����̓o�^�ƃ��X�|���X���e�̌���
		sendBuff = client.header;
		sendBuff << sendContentBuff;

		// ���M
		SendTo(sockfd_, sendBuff, sendBuff.Length(), 0, client.clientAddr_);
	}
}

void MultiServer::Update() {
	REQUEST_PLAYER req = RecvUpdate();
	PlayerUpdate(req);
	SendUpdate();
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
			HEADER *pHeader = (HEADER *)buff;

			switch (pHeader->command)
			{
			case HEADER::COMMAND::REQUEST_LOGIN: {
				// �o�^
				std::cout << "Req << ID:*" << " Login" << std::endl;
				int id = Register(clientAddr, pHeader, 0);
				// ���M
				SendTo(sockfd_, (char *)&clients_[id].header, sizeof(HEADER), 0, clients_[id].clientAddr_);
				std::cout << id << " : �o�^���܂���" << std::endl;
				break;
			};
			case HEADER::COMMAND::REQUEST_LOGOUT: {
				// ����
				std::cout << "Req << ID:" << pHeader->id << " Logout" << std::endl;
				Unregister(pHeader->id);
				std::cout << pHeader->id << " : �������܂���" << std::endl;
				break;
			}
			case HEADER::COMMAND::REQUEST_UPDATE: {
				// �X�V
				recvBuff.Push(buff, buffLen);
				Update();
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
		header_.command = HEADER::COMMAND::REQUEST_LOGIN;

		// ����
		readfd_.Add(sockfd_);

		// ���M
		std::cout << "Req >> ID:*" << " Login" << std::endl;
		SendTo(sockfd_, (char *)&header_, sizeof(HEADER), 0, serverAddr);

		// ��M
		while (true) {
			Recv(sockfd_, (char *)&header_, sizeof(HEADER), 0);
			if (header_.command = HEADER::RESPONSE_LOGIN) break;
		}
		std::cout << "Res << ID:" << header_.id << " Login" << std::endl;
		std::cout << header_.id << "�Ԗڂɓo�^���܂����B" << std::endl;

		return header_.id;
	}

void Client::Unregister() {
		// �R�}���h�ݒ�
	header_.command = HEADER::REQUEST_LOGOUT;

		// ���M
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

		std::cout << header_.id << "�Ԗڂ��������܂����B" << std::endl;

		sockfd_.Close();
	}

void Client::PlayerUpdate(RESPONSE_PLAYER res, RESPONSE_PLAYER reses) {

}

void Client::SendUpdate(void) {
	// ���N�G�X�g�̍쐬
	CreateRequestToServer(sendBuff);

	// ���M
	SendTo(sockfd_, sendBuff, sendBuff.Length(), 0, serverAddr);
}

void Client::RecvUpdate(int waitTime) {
	FD tmp;
	memcpy(&tmp, &readfd_, sizeof(FD));

	// ���m����
	const int MAX_BUFF = 1024;
	if (Select(&tmp, nullptr, nullptr, 0, waitTime)) {
		char buff[MAX_BUFF] = {};

		// ��M����
		int buffLen = Recv(sockfd_, (char *)buff, MAX_BUFF);
		recvBuff.Push(buff, buffLen);

		// ���X�|���X�̉��
		RESPONSE_PLAYER reses[MAX_MEMBER] = {};
		ParseResponseFromServer(recvBuff, reses);

		// �X�V
		for (RESPONSE_PLAYER &res : reses) {
			// �������g���X�V
			if (res.header.id == header_.id) {
				header_ = res.header;
				if (header_.command == HEADER::RESPONSE_LOGOUT) return;
			}
			// ���l���X�V
			else {
				// ID��0~MAX_MEMBER-1�ɂ���
				int id = header_.id;
				if (header_.id <= id) id--;

				// �X�V
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
