#include "multiplay.h"
#include "fire.h"
#include "wind.h"
#include "thunder.h"
#include "dark.h"
#include "multi_character_select_mode.h"

/***********************************************************
	Server
************************************************************/
/// <summary>
/// �������͑I��
/// </summary>
void MultiPlayCharacterSelectModeServerSide::AttributeSelect(Vector2 stick, Vector2 previousStick, ATTRIBUTE_TYPE &attributeType) {
	Vector2 direction = stick - previousStick;					// ���͂̃x�N�g��
	float accelerationX = direction.x;							// X�̉����x
	const float acceleActivate = 0.01f;							// X���͂̔���l
	int type = (int)attributeType;								// ����

	// X���͂ɓ��͂��ꂽ�Ȃ�
	if (stick != Vector2::Zero) {
		int   i = 0;
	}
	if (acceleActivate < MATH::Abs(accelerationX)) {
		// �E�Ɉړ�
		if (0.0f < accelerationX) type = ((type + 1) % 4);
		// ���Ɉړ�
		else type = (4 - ((type - 1) % 4));
		// �����ꑮ�����G���[�l�ɂȂ����ꍇ����������
		if (type < 0 || 4 < type) type = 0;
		attributeType = (ATTRIBUTE_TYPE)type;
	}
}

void MultiPlayCharacterSelectModeServerSide::PlayerUpdate(std::list<CLIENT_DATA_SERVER_SIDE> &clients) {
	for (auto &client : clients) {
		// ���͏����Z�b�g����
		Input::SetState(0, client.currentInput);
		Input::SetPreviousState(0, client.previousInput);

		// �I������
		AttributeSelect(Input::GetStickLeft(0), Input::GetPreviousStickLeft(0), client.moveAttribute);
		AttributeSelect(Input::GetStickRight(0), Input::GetPreviousStickRight(0), client.actionAttribute);
	}
}

Attribute *MultiPlayCharacterSelectModeServerSide::CreateAttribute(ATTRIBUTE_TYPE type, Player* player) {
	switch (type) {
	case ATTRIBUTE_TYPE_FIRE: return new Fire(player);
	case ATTRIBUTE_TYPE_WIND: return new Wind(player);
	case ATTRIBUTE_TYPE_THUNDER: return new Thunder(player);
	case ATTRIBUTE_TYPE_DARK: return new Dark(player);
	}
}


void MultiPlayCharacterSelectModeServerSide::Release(std::list<CLIENT_DATA_SERVER_SIDE> &clients) {
	for (auto &client : clients) {
		// �v���C���[�̌���
		auto iterator = game_->find(client.header.id);

		// ID�����݂��Ȃ��Ȃ珈�����Ȃ�
		if (iterator == game_->GetClients().end()) continue;

		// �ړ������X�V
		{
			Attribute *moveAttribute = CreateAttribute(iterator->moveAttribute, iterator->player_);
			iterator->player_->SetAttribute(moveAttribute);
		}
		// �U�������X�V
		{
			Attribute *actionAttribute = CreateAttribute(iterator->actionAttribute, iterator->player_);
			iterator->player_->SetAttribute(actionAttribute);
		}
	}
}

void MultiPlayCharacterSelectModeServerSide::Update(std::list<CLIENT_DATA_SERVER_SIDE> &clients) {
	//pCharacters_ = &clients;
	//PlayerUpdate(clients);

}

void MultiPlayCharacterSelectModeServerSide::CreateResponse(Storage &out) {
	RESPONSE_CHARACTER_SELECT res;

	if (pCharacters_) {
		// �L�����N�^�[�̐ݒ�
		for (auto &character : *pCharacters_)
			res.characters.push_back({ character.header.id, character.moveAttribute, character.actionAttribute });
	}

	// ���X�|���X�쐬
	res.CreateResponse(out);
}






/***********************************************************
	Client
************************************************************/
void MultiPlayCharacterSelectModeClientSide::CharacterDraw(int idx, int maxIdx, float protrude, float gap, float showAttribute, float showRateMin, float showRateMax) {
	const float SCREEN_WIDTH = Graphical::GetWidth();				// ��ʂ̕�
	const float SCREEN_HEIGHT = Graphical::GetHeight();				// ��ʂ̍���

	// ���ۂɎg���镝 / maxIdx - �L�������̌���
	//float width = (SCREEN_WIDTH) / maxIdx - (gap * (maxIdx - 1) * 0.5f) - gap;	// �����G�̕��i���Ԃ��l���j
	float width = (SCREEN_WIDTH - maxIdx * gap - gap - protrude) / maxIdx;	// �����G�̕��i���Ԃ��l���j
	float height = SCREEN_HEIGHT - 2 * gap;							// �����G�̍���



	// �L�����̕`��
	// ��ʂ̘g�̕���
	float x = gap + (width + gap) * idx;							// �`�悷��X���W

	// �㔼�g or �����g�݂̂�`�悵��������
	// X���W�ǂ��ŋ�؂�̂��v�Z����
	float minHorizontal = MATH::Leap(x + protrude, x, showRateMin);
	float maxHorizontal = MATH::Leap(x + protrude, x, showRateMax);
	// Y���W�ǂ�����ǂ��܂ł�`�悷�邩�v�Z����
	float minVertical = MATH::Leap(gap, SCREEN_HEIGHT - gap, showRateMin);
	float maxVertical = MATH::Leap(gap, SCREEN_HEIGHT - gap, showRateMax);

	// �\������L�����N�^�[�G�̃|���S��
	Vector2 vertices[] = {
		Vector2(minHorizontal + width,	minVertical),	// �E��
		Vector2(minHorizontal,			minVertical),	// ����
		Vector2(maxHorizontal + width,	maxVertical),	// �E��
		Vector2(maxHorizontal,			maxVertical),	// ����
	};


	float aspectRatio = width / height;								// �摜�̃A�X�y�N�g��
	float halfRatio = aspectRatio * 0.5f;							// �A�X�y�N�g��̔���
	float diagonalRatio = protrude / height;						// �L�����N�^�[�G�̌X���̔䗦
	float uCenter = showAttribute * 0.25f + 0.125;					// �\���������L����
	// 4�L������C�ɕ\������邽��
	halfRatio /= 4.0f;
	diagonalRatio /= 4.0f;


	// U���W�ǂ��ŋ�؂�̂��v�Z����
	float minU = MATH::Leap(diagonalRatio * 0.5f, -diagonalRatio * 0.5f, showRateMin);
	float maxU = MATH::Leap(diagonalRatio * 0.5f, -diagonalRatio * 0.5f, showRateMax);
	// �\������L�����N�^�[�G��UV�l
	Vector2 uvs[] = {
		Vector2(uCenter + halfRatio + minU, showRateMin),
		Vector2(uCenter - halfRatio + minU, showRateMin),
		Vector2(uCenter + halfRatio + maxU, showRateMax),
		Vector2(uCenter - halfRatio + maxU, showRateMax),
	};
	for (int i = 0; i < 4; i++) {
		uvs[i].x = uvs[i].x <= 1.0f ? uvs[i].x : 1.0f;
		uvs[i].x = uvs[i].x >= 0.0f ? uvs[i].x : 0.0f;
	}
	// �`��
	DrawSprite(LoadTexture("data/texture/player1_11_22_33_44.png"), vertices, uvs, Color::White);

	// �g�̕`��
	DrawLine(vertices[0], vertices[1], Color::Black);
	DrawLine(vertices[1], vertices[3], Color::Black);
	DrawLine(vertices[3], vertices[2], Color::Black);
	DrawLine(vertices[2], vertices[0], Color::Black);
}

void MultiPlayCharacterSelectModeClientSide::Draw(RESPONSE_PLAYER &players) {
	DebugUI::BeginDraw();
	Text::TextStart();
	prep.Update();
	prep.Draw();
	game_->move_ = (ATTRIBUTE_TYPE)((int)prep.GetMove() * 0.5f);
	game_->action_ = (ATTRIBUTE_TYPE)((int)prep.GetAttack() * 0.5f);
	Text::TextEnd();
	DebugUI::EndDraw();
	//// �L�����N�^�[�����݂��Ȃ��Ȃ珈�����Ȃ�
	//if (players.clients.size() == 0) return;

	//int idx = 0;								// �C���f�b�N�X
	//int playerNum = players.clients.size();		// �v���C���[��
	//int protrude = 100;							// �X��
	//int gap = 50;								// �g�̕�
	//for (auto &c : players.clients) {
	//	auto iterator = characters.find(c.id);
	//	// �o�^
	//	if (iterator == characters.end())
	//		characters[c.id] = AnimData(c.attackAttributeType, c.moveAttributeType);
	//	// �X�V
	//	else iterator->second.set(c.attackAttributeType, c.moveAttributeType);

	//	// �p�����[�^�̍X�V
	//	auto &anim = characters[c.id];
	//	auto &uAttack = anim.uAttackAnim;
	//	auto &uMove = anim.uMoveAnim;
	//	uAttack = c.attackAttributeType;
	//	uMove = c.moveAttributeType;
	//	uAttack.update();
	//	uMove.update();

	//	CharacterDraw(idx, playerNum, protrude, gap, uAttack, 0.0f, 0.5f);
	//	CharacterDraw(idx, playerNum, protrude, gap, uMove, 0.5f, 1.0f);

	//	// �C���f�b�N�X�𑝂₷
	//	idx++;
	//}
}

void MultiPlayCharacterSelectModeClientSide::ParseResponse(Storage &in) {
	res = RESPONSE_CHARACTER_SELECT();
	// ���X�|���X���
	res.ParseResponse(in);
}