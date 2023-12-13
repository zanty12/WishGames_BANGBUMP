#include "multiplay.h"
#include "xinput.h"
#pragma comment(lib, "lib/lib.lib")



int MultiServer::Register(Address clientAddr, HEADER &header, Socket sockfd) {
	Vector2 pos = Vector2::Zero;
	float rot = 0.0f;
	int texNo = 0;
	Vector2 vel = Vector2::Zero;
	Player player = Player(pos, rot, texNo, vel, &mapMngr);

	// �w�b�_�[�̍X�V
	header.command = HEADER::RESPONSE_LOGIN;
	header.id = maxID++;

	// �N���C�A���g�f�[�^�̍쐬
	CLIENT_DATA clientData = {
		header,
		sockfd,
		clientAddr,
		player
	};

	// �v���C���[�ǉ�
	clients_.push_back(clientData);


	// ���M
	header.command = HEADER::RESPONSE_LOGIN;
	SendTo(this->sockfd_, (char *)&header, sizeof(HEADER), 0, clientAddr);
	std::cout << "Res >> ID:" << maxID << " Login" << std::endl;

	return maxID - 1;
}

void MultiServer::Unregister(int id) {

	// �v���C���[�̌���
	auto iterator = find(id);
	
	// �����s��v
	if (iterator == clients_.end()) return;

	// ����
	iterator->header.command = HEADER::RESPONSE_LOGOUT;
	iterator->sockfd_.Close();

	// ���M
	SendTo(sockfd_, (char *)&iterator->header, sizeof(HEADER), 0, iterator->clientAddr_);
	std::cout << "Res >> ID:" << " Logout" << std::endl;

	// �폜
	clients_.erase(iterator);
}

void MultiServer::PlayerUpdate(REQUEST_PLAYER req) {

	// �v���C���[�̌���
	auto iterator = find(req.header.id);

	// �����s��v
	if (iterator == clients_.end()) return;

	// �L�[���͂̍X�V
	Input::SetState(0, req.curInput);
	Input::SetPreviousState(0, req.preInput);

	// �w�b�_�[�̍X�V
	iterator->header = req.header;



	// �v���C���[�̏������s��
	iterator->player_.Update();
}

REQUEST_PLAYER MultiServer::RecvUpdate(void) {
	// ���N�G�X�g���������o��
	REQUEST_PLAYER req;

	ParseRequestFromClient(recvBuff, &req);

	return req;
}

void MultiServer::SendUpdate(void) {
	// ���X�|���X�̍쐬
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
				int id = Register(clientAddr, *pHeader, 0);
				// ���M
				//SendTo(sockfd_, (char *)&clients_[id].header, sizeof(HEADER), 0, clients_[id].clientAddr_);
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
	HEADER &header_ = player.header;

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
	SendTo(sockfd_, (char *)&player.header, sizeof(HEADER), 0, serverAddr);

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
	HEADER &header_ = player.header;

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

	std::cout << header_.id << "���������܂����B" << std::endl;

	sockfd_.Close();
}
#define Cursor(x, y) printf("\033[%d;%dH", y, x)
void Client::PlayerUpdate(RESPONSE_PLAYER &res, std::list<RESPONSE_PLAYER> &reses) {
	system("cls");
	Cursor((int)res.position.x, (int)res.position.y);
	printf("*");

	for (auto &res : reses) {
		Cursor((int)res.position.x, (int)res.position.y);
		printf("*");
	}
}

void Client::SendUpdate(void) {
	// ���N�G�X�g�̍쐬
	CreateRequestToServer(sendBuff);

	// ���M
	SendTo(sockfd_, sendBuff, sendBuff.Length(), 0, serverAddr);
}

void Client::RecvUpdate(int waitTime, RESPONSE_PLAYER &res, std::list<RESPONSE_PLAYER> &reses) {
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
		ParseResponseFromServer(recvBuff, reses);

		// �X�V
		for (RESPONSE_PLAYER &res : reses) {
			// �������g���X�V
			if (res.header.id == player.header.id) {
				player.header = res.header;
				if (player.header.command == HEADER::RESPONSE_LOGOUT) return;
			}
			// ���l���X�V
			else {
				// ID��0~MAX_MEMBER-1�ɂ���
				int id = res.header.id;

				// �v���C���[�̌���
				auto iterator = find(id);

				// �X�V
				if (res.header.command == HEADER::RESPONSE_LOGOUT) res.header = HEADER();
				iterator->header = res.header;
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
