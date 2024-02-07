#include "multiplay.h"
#include "multi_attribute.h"
#include "multi_character_select_mode.h"
#include "move_scene_anim.h"

/***********************************************************
	Server
************************************************************/
/// <summary>
/// �������͑I��
/// </summary>
void MultiPlayCharacterSelectModeServerSide::AttributeSelect(Vector2 stick, Vector2 previousStick, ATTRIBUTE_TYPE &attributeType) {
	Vector2 direction = stick - previousStick;					// ���͂̃x�N�g��
	float triggerJudge = 0.5f;									// ���͔���
	int type = (int)attributeType;								// ����

	// X���͂ɓ��͂��ꂽ�Ȃ�
	if (previousStick.x < +triggerJudge && +triggerJudge <= stick.x ||
		stick.x <= -triggerJudge && -triggerJudge < previousStick.x) {
		// �E�Ɉړ�
		if (0.0f < stick.x) type++;
		// ���Ɉړ�
		else if (stick.x < 0.0f) type--;

		// �������G���[�l�ɂȂ����ꍇ�l�����炷
		if (3 < type) type = type % 4;
		else if (type < 0) type = 4 + (type % 4);
		attributeType = (ATTRIBUTE_TYPE)type;
	}
}

void MultiPlayCharacterSelectModeServerSide::PlayerUpdate(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {
	RESPONSE_CHARACTER_SELECT res;
	bool isSkip = true;

	// �L�����N�^�[�̐ݒ�
	for (auto &kvp : MultiPlayServer::clients_) {
		auto &client = kvp.second;
		// ���͏����Z�b�g����
		Input::SetState(0, client.currentInput);
		Input::SetPreviousState(0, client.previousInput);

		// �I������
		AttributeSelect(Input::GetStickLeft(0), Input::GetPreviousStickLeft(0), client.moveAttributeType);
		AttributeSelect(Input::GetStickRight(0), Input::GetPreviousStickRight(0), client.attackAttributeType);

		// ����
		if (Input::GetKey(0, Input::A)) {
			client.isSkip = true;
		}
		// �L�����Z��
		else if (Input::GetKey(0, Input::B)) {
			client.isSkip = false;
		}

		// �X�L�b�v���Ȃ�
		if (client.isSkip == false) isSkip = false;
	}

	// �S��OK�Ȃ�X�L�b�v����
	if (isSkip) {
		this->isSkip = isSkip;
	}
}

ServerAttribute *MultiPlayCharacterSelectModeServerSide::CreateAttribute(ATTRIBUTE_TYPE type, ServerPlayer* player) {
	switch (type) {
	case ATTRIBUTE_TYPE_FIRE: return new ServerFire(player);
	case ATTRIBUTE_TYPE_WIND: return new ServerWind(player);
	case ATTRIBUTE_TYPE_THUNDER: return new ServerThunder(player);
	case ATTRIBUTE_TYPE_DARK: return new ServerWater(player);
	}
}

void MultiPlayCharacterSelectModeServerSide::Release(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {
	for (auto &kvp : clients) {
		// �v���C���[�̌���
		auto &client = kvp.second;
		auto &player = client.player_;

		// �ړ������X�V
		{
			ServerAttribute *moveAttribute = CreateAttribute(client.moveAttributeType, player);
			player->SetMoveAttribute(moveAttribute);
		}
		// �U�������X�V
		{
			ServerAttribute *attackAttribute = CreateAttribute(client.attackAttributeType, player);
			player->SetAttackAttribute(attackAttribute);
		}
	}
}

void MultiPlayCharacterSelectModeServerSide::Update(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {
	PlayerUpdate(clients);

}

void MultiPlayCharacterSelectModeServerSide::CreateResponse(Storage &out) {
	RESPONSE_CHARACTER_SELECT res;

	// �L�����N�^�[�̐ݒ�
	for (auto &kvp : MultiPlayServer::clients_) {
		auto &client = kvp.second;
		res.characters.push_back({ client.header.id, client.moveAttributeType, client.attackAttributeType });
	}

	// ���X�|���X�쐬
	res.CreateResponse(out);
}







/***********************************************************
	Client
************************************************************/

ClientAttribute *MultiPlayCharacterSelectModeClientSide::CreateAttribute(ATTRIBUTE_TYPE type, ClientPlayer *player){
	switch (type) {
	case ATTRIBUTE_TYPE_FIRE: return new ClientFire(player);
	case ATTRIBUTE_TYPE_WIND: return new ClientWind(player);
	case ATTRIBUTE_TYPE_THUNDER: return new ClientThunder(player);
	case ATTRIBUTE_TYPE_DARK: return new ClientWater(player);
	}
}

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

void MultiPlayCharacterSelectModeClientSide::DrawStart(RESPONSE_PLAYER &players, Vector2 offset) {
	MoveScene::Move(0.0f);
}

void MultiPlayCharacterSelectModeClientSide::DrawResult(RESPONSE_PLAYER &players, Vector2 offset) {
	MoveScene::Move(1.0f);
}

void MultiPlayCharacterSelectModeClientSide::Draw(RESPONSE_PLAYER &players, Vector2 offset) {
	//DebugUI::BeginDraw();
	//Text::TextStart();
	//prep.Update();
	//prep.Draw();
	//game_->move_ = (ATTRIBUTE_TYPE)((int)prep.GetMove() * 0.5f);
	//game_->action_ = (ATTRIBUTE_TYPE)((int)prep.GetAttack() * 0.5f);
	//Text::TextEnd();
	//DebugUI::EndDraw();
	ATTRIBUTE_TYPE moveAttributeType, attackAttributeType;

	int idx = 0;
	for (auto &character : res.characters) {
		int id = character.id;
		if (characters.find(id) == characters.end()) characters[id] = AnimData();

		// ������ID�Ȃ瑮�����L�^����
		if (game_->GetID() == id) {
			moveAttributeType = character.moveAttributeType;
			attackAttributeType = character.attackAttributeType;
		}

		// �A�j���[�V������UV�l�̍X�V
		characters[id].uMoveAnim = character.moveAttributeType;
		characters[id].uAttackAnim = character.attackAttributeType;
		characters[id].uMoveAnim.update();
		characters[id].uAttackAnim.update();

		// �A�j���[�V�����̕`��
		CharacterDraw(idx, res.characters.size(), 0, 10, characters[id].uAttackAnim, 0.0f, 0.6f);
		CharacterDraw(idx, res.characters.size(), 0, 10, characters[id].uMoveAnim, 0.6f, 1.0f);
		idx++;
	}

	// ����̕`��
	if (0.75f < Input::GetTriggerLeft(0) && 0.75f < Input::GetTriggerRight(0)) {
		DebugUI::BeginDraw();
		Text::TextStart();
		video->Update();
		video->Draw();
		Text::TextEnd();
		DebugUI::EndDraw();
	}
}

void MultiPlayCharacterSelectModeClientSide::ParseResponse(Storage &in) {
	res = RESPONSE_CHARACTER_SELECT();
	// ���X�|���X���
	res.ParseResponse(in);
}

void MultiPlayCharacterSelectModeClientSide::Release(RESPONSE_PLAYER &players) {
	for (auto &client : players.clients) {
		// �v���C���[�̌���
		auto &player = game_->clients[client.id];

		// �ړ������X�V
		{
			ClientAttribute *moveAttribute = CreateAttribute(client.moveAttributeType, player);
			if (player->GetMoveAttribute()) delete player->GetMoveAttribute();
			player->SetMoveAttribute(moveAttribute);
		}
		// �U�������X�V
		{
			ClientAttribute *attackAttribute = CreateAttribute(client.attackAttributeType, player);
			if (player->GetAttackAttribute()) delete player->GetAttackAttribute();
			player->SetAttackAttribute(attackAttribute);
		}
	}
}
