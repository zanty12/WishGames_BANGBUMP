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

		int &refStatus = status[client.player_->id];
		// ����
		if (Input::GetKeyDown(0, Input::A)) {
			refStatus++;
			if (2 < refStatus) refStatus = 2;
		}
		// �L�����Z��
		else if (Input::GetKeyDown(0, Input::B)) {
			refStatus--;
			if (refStatus < 0) refStatus = 0;			
		}


		// �X�L�b�v
		client.isSkip = refStatus == 2;
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

		if (client.moveAttributeType == client.attackAttributeType) {
			client.attackAttributeType = (ATTRIBUTE_TYPE)((client.attackAttributeType + 1) % ATTRIBUTE_TYPE_NUM);
		}
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

void MultiPlayCharacterSelectModeServerSide::UpdateResult(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {
}

void MultiPlayCharacterSelectModeServerSide::CreateResponse(Storage &out) {
	RESPONSE_CHARACTER_SELECT res;

	// �L�����N�^�[�̐ݒ�
	for (auto &kvp : MultiPlayServer::clients_) {
		auto &client = kvp.second;
		res.characters.push_back({ client.header.id, client.moveAttributeType, client.attackAttributeType, status[client.player_->id]});
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

void MultiPlayCharacterSelectModeClientSide::CharacterDraw(int id, int maxIdx, bool isShow, float width, float height, float gap, int moveAttribute, int attackAttribute, float moveAttributeSmooth, float attackAttributeSmooth) {
	const float SCREEN_WIDTH = Graphical::GetWidth();					// ��ʂ̕�
	const float SCREEN_HEIGHT = Graphical::GetHeight();					// ��ʂ̍���
	const float center = (float)maxIdx - (float)maxIdx * 0.5f - 0.5f;	// ����

	float x = (SCREEN_WIDTH * 0.5f) - (center - id) * (width + gap);
	float y = (SCREEN_HEIGHT * 0.5f);

	float scaleMoveAttributeRate = MATH::Abs(moveAttribute - moveAttributeSmooth) + 1.0f;
	float scaleAttackAttributeRate = MATH::Abs(attackAttribute - attackAttributeSmooth) + 1.0f;

	// �`��i�t���[���j
	DrawSprite(charFrameTexNo, Vector2(x, y), 0.0f, Vector2::One * (height + 300), Color::White);
	// �`��i�L�����j
	if (isShow) {
		DrawSprite(charFramePTexNo[id % 4], Vector2(x, y), 0.0f, Vector2::One * (height + 300), Color::White);
		DrawSprite(playerTexNo[id % 4], Vector2(x, y + 40), 0.0f, Vector2::One * height, Color::White);
		DrawSprite(bootTexNo[moveAttribute % ATTRIBUTE_TYPE_NUM], Vector2(x, y + 40), 0.0f, Vector2::One * height * scaleMoveAttributeRate, Color::White);
		DrawSprite(handTexNo[attackAttribute % ATTRIBUTE_TYPE_NUM], Vector2(x, y + 40), 0.0f, Vector2::One * height * scaleAttackAttributeRate, Color::White);
	}
}

void MultiPlayCharacterSelectModeClientSide::DrawStart(RESPONSE_PLAYER &players, Vector2 offset) {
	MoveScene::Move(Color::White * 0.0f);
}

void MultiPlayCharacterSelectModeClientSide::DrawResult(RESPONSE_PLAYER &players, Vector2 offset) {
	MoveScene::Move(Color::Black, true);
}

void MultiPlayCharacterSelectModeClientSide::Draw(RESPONSE_PLAYER &players, Vector2 offset) {
	ATTRIBUTE_TYPE moveAttributeType, attackAttributeType;

	for (int id = 0; id < MAX_MEMBER; id++) {

		float width = 600.0f;
		float height = 600.0f;
		float gap = -120.0f;


			

		// �o�^����Ă��邩���ׂ�
		auto iterator = res.characters.end();
		for (auto tmp = res.characters.begin(); tmp != res.characters.end(); tmp++)
			if (tmp->id == id) iterator = tmp;

		// �f�[�^�̍X�V
		characters[id].id = id;

		// �L�����\��
		if (iterator != res.characters.end()) {
			// �X�e�[�^�X�̍X�V
			auto &character = characters[id];
			character.stateSmooth = iterator->status;


			// ������ID�Ȃ瑮�����L�^����
			if (MultiPlayClient::GetID() == id) {
				moveAttributeType = iterator->moveAttributeType;
				attackAttributeType = iterator->attackAttributeType;
			}

			// �`��
			characters[id].Draw(charFrameTexNo, charFramePTexNo[id % 4], charSelectArrow, playerTexNo[id % 4], bootTexNo[iterator->moveAttributeType], handTexNo[iterator->attackAttributeType], charReadyTexNo,
				true, width, height, gap, iterator->moveAttributeType, iterator->attackAttributeType);
		}
		// �g�̂ݕ\��
		else {
			characters[id].Draw(charFrameTexNo, charFramePTexNo[id % 4], charSelectArrow, playerTexNo[id % 4], bootTexNo[ATTRIBUTE_TYPE_FIRE], handTexNo[ATTRIBUTE_TYPE_FIRE], charReadyTexNo,
				false, width, height, gap, ATTRIBUTE_TYPE_FIRE, ATTRIBUTE_TYPE_FIRE);
		}
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

void MultiPlayCharacterSelectModeClientSide::DrawUI(RESPONSE_PLAYER &players) {

	//�߂�
	DrawSprite(return_tex_, Vector2(((Graphical::GetWidth() - 100)), 70), 0.0f,
		Vector2(80.0f, 80.0f), Color::White);
	DrawSprite(b_tex_, Vector2(((Graphical::GetWidth() - 160)), 70), 0.0f,
		Vector2(100.0f, 100.0f), Color::White);
	//����
	DrawSprite(confirm_tex_, Vector2(((Graphical::GetWidth() - 240)), 70), 0.0f,
		Vector2(80.0f, 80.0f), Color::White);
	DrawSprite(a_tex_, Vector2(((Graphical::GetWidth() - 305)), 70), 0.0f,
		Vector2(100.0f, 100.0f), Color::White);
	//�I��
	DrawSprite(select_tex_, Vector2(((Graphical::GetWidth() - 390)), 70), 0.0f,
		Vector2(80.0f, 80.0f), Color::White);
	DrawSprite(stick_tex_, Vector2(((Graphical::GetWidth() - 475)), 72), 0.0f,
		Vector2(115.0f, 118.0f), Color::White);
	// �}�b�`���O
	DrawSprite(match_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() - 70), 0.0f, // arbirary value of the texture
		Vector2(650, 650), Color(1.0f, 1.0f, 1.0f, 1.0f));
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
