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
	std::cout << "Res >> ID:" << (maxID - 1) << " Login" << std::endl;

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
	iterator->player_->Update();
	std::cout << clients_.size() << std::endl;
	//std::cout << iterator->player_->GetPos().x << ", " << iterator->player_->GetPos().y << std::endl;
}

REQUEST_PLAYER MultiServer::RecvUpdate(void) {
	// ���N�G�X�g���������o��
	REQUEST_PLAYER req;

	ParseRequestFromClient(recvBuff, req);

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
	MSG msg;
	while (true) {
		// ���b�Z�[�W
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

		recvBuff = nullptr;
		sendBuff = nullptr;
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
		// ���s�Ȃ�I��
		if (buffLen <= 0) return;
		recvBuff.Push(buff, buffLen);

		// ���X�|���X�̉��
		std::list<RESPONSE_PLAYER> clients;
		ParseResponseFromServer(recvBuff, clients);

		// �X�V
		for (RESPONSE_PLAYER &r : clients) {
			// �������g���X�V
			if (r.header.id == player.header.id) {
				player.header = r.header;
				player.position = r.position;

				res = player;
				if (player.header.command == HEADER::RESPONSE_LOGOUT) return;
			}
			// ���l���X�V
			else {
				// �X�V
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
