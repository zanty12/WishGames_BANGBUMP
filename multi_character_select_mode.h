#pragma once
#include "multi_mode.h"
#include "lib/math.h"

class MultiPlayCharacterSelectModeServerSide : public MultiPlayServerSide {
private:
	std::list<CLIENT_DATA_SERVER_SIDE> * pCharacters_ = nullptr;



private:
	/// <summary>
	/// �������͑I��
	/// </summary>
	void AttributeSelect(Vector2 stick, Vector2 previousStick, ATTRIBUTE_TYPE &attributeType) {
		Vector2 direction = stick - previousStick;					// ���͂̃x�N�g��
		float accelerationX = direction.x;							// X�̉����x
		const float acceleActivate = 0.3f;							// X���͂̔���l
		int type = (int)attributeType;								// ����

		// X���͂ɓ��͂��ꂽ�Ȃ�
		if (acceleActivate < MATH::Abs(accelerationX)) {
			// �E�Ɉړ�
			if (0.0f < accelerationX) type = (type + 1) % 4;
			// ���Ɉړ�
			else type = 4 - (type - 1) % 4;
			attributeType = (ATTRIBUTE_TYPE)attributeType;
		}
	}

	void PlayerUpdate(std::list<CLIENT_DATA_SERVER_SIDE> &clients) {
		for (auto client : clients) {
			// ���͏����Z�b�g����
			Input::SetState(0, client.currentInput);
			Input::SetState(0, client.previousInput);

			// �I������
			AttributeSelect(Input::GetStickLeft(0), Input::GetPreviousStickLeft(0), client.moveAttribute);
			AttributeSelect(Input::GetStickRight(0), Input::GetPreviousStickRight(0), client.actionAttribute);
		}
	}

public:
	MultiPlayCharacterSelectModeServerSide()
		: MultiPlayServerSide(nullptr) {
		maxTime_ = 1000;
	}

	void Update(std::list<CLIENT_DATA_SERVER_SIDE> &clients) override {
		pCharacters_ = &clients;
		PlayerUpdate(clients);
	}

	void CreateResponse(Storage &out) override {
		RESPONSE_CHARACTER_SELECT res;

		if (pCharacters_) {
			// �L�����N�^�[�̐ݒ�
			for (auto &character : *pCharacters_)
				res.characters.push_back({ character.header.id, character.moveAttribute, character.actionAttribute });
		}

		// ���X�|���X�쐬
		res.CreateResponse(out);
	}

	MULTI_MODE GetMode(void) const override { return CHARACTER_SELECT; }
};






class MultiPlayCharacterSelectModeClientSide : public MultiPlayClientSide {
private:
	RESPONSE_CHARACTER_SELECT res;


public:
	MultiPlayCharacterSelectModeClientSide() : MultiPlayClientSide(nullptr) { }

	void Draw(RESPONSE_PLAYER& players) override {
		// �L�����N�^�[�����݂��Ȃ��Ȃ珈�����Ȃ�
		if (res.characters.size() == 0) return;

		const int SCREEN_WIDTH = Graphical::GetWidth();					// ��ʂ̕�
		const int SCREEN_HEIGHT = Graphical::GetHeight();				// ��ʂ̍���
		const int SCREEN_HORIZONTAL_CENTER = SCREEN_WIDTH * 0.5f;		// ��ʂ̉����̒��S
		const int SCREEN_VERTICAL_CENTER = SCREEN_HEIGHT * 0.5f;		// ��ʂ̏c���̒��S
		const int SCREEN_FRAME_WIDTH = 100;								// ��ʂ̘g�̑���
		const int CHARACTER_FRAME = 50;									// �\������L�����N�^�[�G�̘g�̑���
		const int CHARACTER_DIAGONAL = 100;								// �\������L�����N�^�[�G�̌X���iX�s�N�Z���̃Y���j

		int CHARACTER_WIDTH = (SCREEN_WIDTH - SCREEN_FRAME_WIDTH * 2 - CHARACTER_FRAME - CHARACTER_DIAGONAL) / res.characters.size();			// �\������L�����N�^�[�G�̕�
		

		// �L�����̕`��
		// ��ʂ̘g�̕���
		int x = SCREEN_FRAME_WIDTH;										// �`�悷��X���W
		for (auto character : res.characters) {
			// �\������L�����N�^�[�G�̃|���S��
			Vector2 vertices[] = {
				Vector2(x + CHARACTER_WIDTH + CHARACTER_DIAGONAL,	SCREEN_FRAME_WIDTH),
				Vector2(x + CHARACTER_DIAGONAL,						SCREEN_FRAME_WIDTH),
				Vector2(x + CHARACTER_WIDTH,						SCREEN_HEIGHT - SCREEN_FRAME_WIDTH),
				Vector2(x,											SCREEN_HEIGHT - SCREEN_FRAME_WIDTH),
			};


			float width = CHARACTER_WIDTH;								// �摜�̕�
			float height = SCREEN_HEIGHT - 2 * SCREEN_FRAME_WIDTH;		// �摜�̍���
			float aspectRatio = width / height;							// �摜�̃A�X�y�N�g��
			float halfRatio = aspectRatio * 0.5f;						// �A�X�y�N�g��̔���
			float diagonalRatio = CHARACTER_DIAGONAL / height;			// �L�����N�^�[�G�̌X���̔䗦

			// �\������L�����N�^�[�G��UV�l
			Vector2 uvs[] = {
				Vector2(0.5f + halfRatio + diagonalRatio,	0),
				Vector2(0.5f - halfRatio + diagonalRatio,	0),
				Vector2(0.5f + halfRatio,					1),
				Vector2(0.5f - halfRatio,					1),
			};
			for (int i = 0; i < 4; i++) {
				uvs[i].x = uvs[i].x <= 1.0f ? uvs[i].x : 1.0f;
				uvs[i].x = uvs[i].x >= 0.0f ? uvs[i].x : 0.0f;
			}
			// �`��
			DrawSprite(0, vertices, uvs, Color::White);

			// �g�̕`��
			DrawLine(vertices[0], vertices[1], Color::Black);
			DrawLine(vertices[1], vertices[3], Color::Black);
			DrawLine(vertices[3], vertices[2], Color::Black);
			DrawLine(vertices[2], vertices[0], Color::Black);

			// �L�����N�^�[�G�̕��̕���
			x += width;

			// �L�����N�^�[�G�̘g�̕���
			x += CHARACTER_FRAME;
		}
	}

	void ParseResponse(Storage &in) override {
		res = RESPONSE_CHARACTER_SELECT();
		// ���X�|���X���
		res.ParseResponse(in);
	}

	MULTI_MODE GetMode(void) const override { return CHARACTER_SELECT; }
};
