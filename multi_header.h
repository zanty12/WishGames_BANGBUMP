#pragma once
#include "lib/network.h"
#include "multi_player.h"
#include "multi_animenum.h"
#include "multi_objenum.h"
#include "attribute_type.h"
#include "storage.h"
#include "xinput.h"
#include "attribute.h"

using namespace Network;

enum MULTI_MODE {
	NONE = -1,
	CHARACTER_SELECT = 0,
	AREA_CAPTURE,
	//INTERMEDIATE_RESULT_1,
	//OBSTACLE_RACE,
	//INTERMEDIATE_RESULT_2,
	ENEMY_RUSH,
	//INTERMEDIATE_RESULT_3,
	FINAL_BATTLE,
};


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



	int id = -1;
	int command = NONE;
};
// �N���C�A���g�f�[�^�i�N���C�A���g���j
struct CLIENT_DATA_CLIENT_SIDE {
	int id = -1;												// ID
	ATTRIBUTE_TYPE moveAttributeType = ATTRIBUTE_TYPE_FIRE;		// �ړ������^�C�v
	ATTRIBUTE_TYPE attackAttributeType = ATTRIBUTE_TYPE_FIRE;	// �U�������^�C�v
	MULTI_ANIMATION_TYPE animType;								// �A�j���[�V�����^�C�v
	Vector2 position;											// ���W
	Vector2 moveVelocity;										// �ړ��x���V�e�B
	Vector2 attackVelocity;										// �U���x���V�e�B
	Vector2 warpVelocity;										// ���[�v�x���V�e�B
	int score = 0;												// �X�R�A
	int skillPoint = 0;											// �X�L���|�C���g
	int previousSkillPoint = 0;									// 1�Q�[���O�̃X�L���|�C���g
};
// �N���C�A���g�f�[�^�i�T�[�o�[���j
struct CLIENT_DATA_SERVER_SIDE {
	HEADER header;
	Socket sockfd_;
	Address clientAddr_;										// �N���C�A���g�A�h���X
	ATTRIBUTE_TYPE moveAttributeType = ATTRIBUTE_TYPE_FIRE;		// �ړ������^�C�v
	ATTRIBUTE_TYPE attackAttributeType = ATTRIBUTE_TYPE_FIRE;	// �U�������^�C�v
	ServerPlayer* player_ = nullptr;							// �v���C���[
	bool isSkip = false;										// �X�L�b�v
	XINPUT_GAMEPAD currentInput = XINPUT_GAMEPAD();
	XINPUT_GAMEPAD previousInput = XINPUT_GAMEPAD();

	CLIENT_DATA_SERVER_SIDE() = default;
	CLIENT_DATA_SERVER_SIDE(HEADER header, Socket sockfd, Address addr, ServerPlayer *player)
		: header(header), sockfd_(sockfd), clientAddr_(addr), player_(player) { };
};
// �I�u�W�F�N�g�f�[�^�i�N���C�A���g���j
struct OBJECT_DATA_CLIENT_SIDE {
	enum {
		NONE = -1,
	};



	int id = -1;
	int tag = NONE;
	int anim = NONE;
	Vector2 position;
	Vector2 velocity;
};





// ���X�|���X�i�v���C���[�j
struct RESPONSE_PLAYER {
	typedef CLIENT_DATA_CLIENT_SIDE DESC;
	MULTI_MODE mode = MULTI_MODE::NONE;
	float maxTime = 0;
	float time = 0;
	std::list<CLIENT_DATA_CLIENT_SIDE> clients;
	std::list<OBJECT_DATA_CLIENT_SIDE> objects;

	void CreateResponse(Storage& out, int id) {
		// ������
		out = nullptr;

		// �w�b�_�[�쐬
		HEADER header;
		header.id = id;
		header.command = HEADER::RESPONSE_UPDATE;

		// ���X�|���X�쐬�i�w�b�_�[�j
		out << header;
		out << mode;
		out << maxTime;
		out << time;
		out << clients.size();
		out << objects.size();

		// ���X�|���X�쐬�i�v���C���[�j
		for (auto& client : clients) {
			// ���X�|���X���̐ݒ�
			out << client;
		}

		// ���X�|���X�쐬�i�I�u�W�F�N�g�j
		for (auto& object : objects) {
			// ���X�|���X���̐ݒ�
			out << object;
		}
	}

	void ParseResponse(Storage& in) {
		HEADER recvHeader;
		size_t playerNum = 0;
		size_t objectNum = 0;

		// �w�b�_�[�擾
		in >> recvHeader;
		in >> mode;
		in >> maxTime;
		in >> time;
		in >> playerNum;
		in >> objectNum;
		int current = in.Current();

		// ���X�|���X��́i�v���C���[�j
		for (int i = 0; i < playerNum; i++) {
			CLIENT_DATA_CLIENT_SIDE res;
			in >> res;
			current = in.Current();

			// �����Ȃ�擪��
			if (res.id == recvHeader.id) {
				clients.push_front(res);
			}
			else {
				clients.push_back(res);
			}
		}

		// ���X�|���X��́i�I�u�W�F�N�g�j
		for (int i = 0; i < objectNum; i++) {
			OBJECT_DATA_CLIENT_SIDE res;
			in >> res;
			current = in.Current();
			objects.push_back(res);
		}
	}

	void AddObjects(MultiBehavior *list){
		for (auto &instance : *list) {
			auto gameObject = instance.Cast<GameObjectServerSide>();
			int id = gameObject->id;
			Vector2 position = gameObject->transform.position;
			Vector2 velocity = gameObject->velocity;
			objects.push_back({
				id,										// ID
				gameObject->GetType(),					// tag
				0,										// animation
				position,								// pos
				velocity								// vel
				}
			);
		}
	}
};
// ���N�G�X�g�i�v���C���[�j
struct REQUEST_PLAYER {
	struct DESC {
		int id;
		XINPUT_GAMEPAD curInput;
		XINPUT_GAMEPAD preInput;
		ATTRIBUTE_TYPE move = ATTRIBUTE_TYPE_FIRE;
		ATTRIBUTE_TYPE action = ATTRIBUTE_TYPE_FIRE;

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



// ���X�|���X�i�L�����N�^�[�I���j
struct RESPONSE_CHARACTER_SELECT {
	struct DESC {
		int id = -1;						// ������ID
		ATTRIBUTE_TYPE moveAttributeType;	// �ړ������^�C�v
		ATTRIBUTE_TYPE attackAttributeType;	// �U�������^�C�v
	};

	std::list<DESC> characters;

	void CreateResponse(Storage &out) {
		// ���X�|���X�쐬�i�w�b�_�[�j
		out << characters.size();

		// ���X�|���X�쐬�i�{�f�B�j
		for (DESC &character : characters) {
			out << character;
		}
	}

	void ParseResponse(Storage &in) {
		size_t characterNum;

		// �w�b�_�[�擾
		in >> characterNum;

		// �{�f�B�擾
		for (int i = 0; i < characterNum; i++) {
			DESC area;
			in >> area;
			characters.push_back(area);
		}
	}
};
// ���X�|���X�i��̃��[�h�j
struct RESPONSE_AREA_CAPTURE {
	struct DESC {
		Vector2 position;				// ��̂̍��W
		float radius = 700;				// ��̂̃X�P�[��
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
//// ���X�|���X�i���ԃ��U���g�j
//struct RESPONSE_INTERMEDIATE_RESULT {
//	struct DESC {
//		int previousSkillPoint = 0;
//	};
//
//	std::list<DESC> 
//};