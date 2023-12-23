#pragma once
#include "multi_mode.h"
#include "lib/math.h"

class MultiPlayCharacterSelectModeServerSide : public MultiPlayServerSide {
private:
	std::list<CLIENT_DATA_SERVER_SIDE> * pCharacters_ = nullptr;

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
	MultiPlayCharacterSelectModeServerSide() : MultiPlayServerSide(nullptr) { };

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
};






class MultiPlayCharacterSelectModeClientSide : public MultiPlayClientSide {
private:
	RESPONSE_CHARACTER_SELECT res;


public:
	MultiPlayCharacterSelectModeClientSide() : MultiPlayClientSide(nullptr) { }

	void Draw(void) override {
		// �G���A�̕`��
		for (auto character : res.characters) {
			//anim.SetTexNo(areaTexNo);
			//anim.SetPos(area.position);
			//anim.SetScale(Vector2(area.radius, area.radius));
			//anim.Draw();
		}
	}

	void ParseResponse(Storage &in) override {
		res = RESPONSE_CHARACTER_SELECT();
		// ���X�|���X���
		res.ParseResponse(in);
	}
};
