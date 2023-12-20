#pragma once
#include "lib/network.h"
#include "storage.h"
#include "player.h"
#include "xinput.h"

using namespace Network;

// �w�b�_�[
struct HEADER {
	enum {
		NONE = -1,
	};

	enum COMMAND {
		REQUEST_LOGIN,
		RESPONSE_LOGIN,
		REQUEST_LOGOUT,
		RESPONSE_LOGOUT,
		REQUEST_UPDATE,
		RESPONSE_UPDATE
	};
	enum SCENE {
		SELECT,
		AREA_CAPTURE
	};


	int id = -1;
	int command = NONE;
	int scene = NONE;
};
// �N���C�A���g�f�[�^�i�N���C�A���g���j
struct CLIENT_DATA_CLIENT_SIDE {
	int id = -1;
	Vector2 position;
};
// �N���C�A���g�f�[�^�i�T�[�o�[���j
struct CLIENT_DATA_SERVER_SIDE {
	HEADER header;
	Socket sockfd_;
	Address clientAddr_;
	Player* player_;
};





// ���X�|���X�i�v���C���[�j
struct RESPONSE_PLAYER {
	typedef CLIENT_DATA_CLIENT_SIDE DESC;

	std::list<CLIENT_DATA_CLIENT_SIDE> clients;

	void CreateResponse(Storage& out, int id) {
		// ������
		out = nullptr;

		// �w�b�_�[�쐬
		HEADER header;
		header.id = id;
		header.command = HEADER::RESPONSE_UPDATE;

		// ���X�|���X�쐬�i�w�b�_�[�j
		out << header;
		out << clients.size();

		// ���X�|���X�쐬�i�v���C���[�j
		for (auto& client : clients) {
			// ���X�|���X���̐ݒ�
			out << client;
		}
	}

	void ParseResponse(Storage& in) {
		HEADER recvHeader;
		size_t playerNum = 0;

		// �w�b�_�[�擾
		in >> recvHeader;
		in >> playerNum;


		// ���X�|���X��́i�v���C���[�j
		for (int i = 0; i < playerNum; i++) {
			CLIENT_DATA_CLIENT_SIDE res;
			in >> res;

			// �����Ȃ�擪��
			if (res.id == recvHeader.id) {
				clients.push_front(res);
			}
			else {
				clients.push_back(res);
			}
		}

		// ������
		in = nullptr;
	}
};
// ���N�G�X�g�i�v���C���[�j
struct REQUEST_PLAYER {
	struct DESC {
		int id;
		XINPUT_GAMEPAD curInput;
		XINPUT_GAMEPAD preInput;
	};
	DESC input;

	void CreateRequest(Storage& out, int id) {
		// ������
		out = nullptr;

		// �w�b�_�[�쐬
		HEADER header;
		header.id = id;
		header.command = HEADER::REQUEST_UPDATE;

		// ���X�|���X�쐬�i�w�b�_�[�j
		out << header;

		// ���X�|���X�쐬�i���́j
		out << input;
	}

	void ParseRequest(Storage& in) {
		HEADER recvHeader;

		// �w�b�_�[�擾
		in >> recvHeader;


		// ���X�|���X��́i���́j
		in >> input;

		// ������
		in = nullptr;
	}
};


// ���X�|���X�i��̃��[�h�j
struct RESPONSE_AREA_CAPTURE {
	struct DESC {
		Vector2 position;				// ��̂̍��W
		Vector2 scale;					// ��̂̃X�P�[��
		float captureRatio = 0.0f;		// ��̓x����
		int id = -1;					// ���ݐ�̂��Ă���ID
	};

	std::list<DESC> areas;

	void CreateResponse(Storage& out) {
		// ���X�|���X�쐬�i�w�b�_�[�j
		out << areas.size();

		// ���X�|���X�쐬�i�{�f�B�j
		for (DESC& area : areas) {
			out << area;
		}
	}

	void ParseResponse(Storage& in) {
		size_t areaNum;

		// �w�b�_�[�擾
		in >> areaNum;

		// �{�f�B�擾
		for (int i = 0; i < areaNum; i++) {
			DESC area;
			in >> area;
			areas.push_back(area);
		}
	}
};
