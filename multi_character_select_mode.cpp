#include "multiplay.h"
#include "multi_attribute.h"
#include "multi_character_select_mode.h"
#include "move_scene_anim.h"

/***********************************************************
	Server
************************************************************/
/// <summary>
/// 属性入力選択
/// </summary>
void MultiPlayCharacterSelectModeServerSide::AttributeSelect(Vector2 stick, Vector2 previousStick, ATTRIBUTE_TYPE &attributeType) {
	Vector2 direction = stick - previousStick;					// 入力のベクトル
	float triggerJudge = 0.5f;									// 入力判定
	int type = (int)attributeType;								// 属性

	// X入力に入力されたなら
	if (previousStick.x < +triggerJudge && +triggerJudge <= stick.x ||
		stick.x <= -triggerJudge && -triggerJudge < previousStick.x) {
		// 右に移動
		if (0.0f < stick.x) type++;
		// 左に移動
		else if (stick.x < 0.0f) type--;

		// 属性がエラー値になった場合値をずらす
		if (3 < type) type = type % 4;
		else if (type < 0) type = 4 + (type % 4);
		attributeType = (ATTRIBUTE_TYPE)type;
	}
}

void MultiPlayCharacterSelectModeServerSide::PlayerUpdate(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) {
	RESPONSE_CHARACTER_SELECT res;
	bool isSkip = true;

	// キャラクターの設定
	for (auto &kvp : MultiPlayServer::clients_) {
		auto &client = kvp.second;
		// 入力情報をセットする
		Input::SetState(0, client.currentInput);
		Input::SetPreviousState(0, client.previousInput);

		// 選択する
		AttributeSelect(Input::GetStickLeft(0), Input::GetPreviousStickLeft(0), client.moveAttributeType);
		AttributeSelect(Input::GetStickRight(0), Input::GetPreviousStickRight(0), client.attackAttributeType);

		// 決定
		if (Input::GetKey(0, Input::A)) {
			client.isSkip = true;
		}
		// キャンセル
		else if (Input::GetKey(0, Input::B)) {
			client.isSkip = false;
		}

		// スキップしない
		if (client.isSkip == false) isSkip = false;
	}

	// 全員OKならスキップする
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
		// プレイヤーの検索
		auto &client = kvp.second;
		auto &player = client.player_;

		// 移動属性更新
		{
			ServerAttribute *moveAttribute = CreateAttribute(client.moveAttributeType, player);
			player->SetMoveAttribute(moveAttribute);
		}
		// 攻撃属性更新
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

	// キャラクターの設定
	for (auto &kvp : MultiPlayServer::clients_) {
		auto &client = kvp.second;
		res.characters.push_back({ client.header.id, client.moveAttributeType, client.attackAttributeType });
	}

	// レスポンス作成
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

void MultiPlayCharacterSelectModeClientSide::CharacterDraw(int id, int maxIdx, float width, float height, float gap, int moveAttribute, int attackAttribute, float moveAttributeSmooth, float attackAttributeSmooth) {
	const float SCREEN_WIDTH = Graphical::GetWidth();					// 画面の幅
	const float SCREEN_HEIGHT = Graphical::GetHeight();					// 画面の高さ
	const float center = (float)maxIdx - (float)maxIdx * 0.5f - 0.5f;	// 中央

	float x = (SCREEN_WIDTH * 0.5f) - (id - center) * (width + gap);
	float y = (SCREEN_HEIGHT * 0.5f);

	float scaleMoveAttributeRate = MATH::Abs(moveAttribute - moveAttributeSmooth) + 1.0f;
	float scaleAttackAttributeRate = MATH::Abs(attackAttribute - attackAttributeSmooth) + 1.0f;

	// 描画
	DrawSprite(playerTexNo[id % 4], Vector2(x, y), 0.0f, Vector2::One * height, Color::White);
	DrawSprite(bootTexNo[moveAttribute % ATTRIBUTE_TYPE_NUM], Vector2(x, y), 0.0f, Vector2::One * height * scaleMoveAttributeRate, Color::White);
	DrawSprite(handTexNo[attackAttribute % ATTRIBUTE_TYPE_NUM], Vector2(x, y), 0.0f, Vector2::One * height * scaleAttackAttributeRate, Color::White);
}

void MultiPlayCharacterSelectModeClientSide::DrawStart(RESPONSE_PLAYER &players, Vector2 offset) {
	MoveScene::Move(Color::White * 0.0f);
}

void MultiPlayCharacterSelectModeClientSide::DrawResult(RESPONSE_PLAYER &players, Vector2 offset) {
	MoveScene::Move(Color::White * 1.0f);
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

		// 自分のIDなら属性を記録する
		if (game_->GetID() == id) {
			moveAttributeType = character.moveAttributeType;
			attackAttributeType = character.attackAttributeType;
		}

		// アニメーションのUV値の更新
		characters[id].uMoveAnim = character.moveAttributeType;
		characters[id].uAttackAnim = character.attackAttributeType;
		characters[id].uMoveAnim.update();
		characters[id].uAttackAnim.update();

		// アニメーションの描画
		float width = 150.0f;
		float height = 900.0f;
		float gap = 50.0f;
		CharacterDraw(
			id, res.characters.size(), 
			width, height, gap,
			character.moveAttributeType, character.attackAttributeType, characters[id].uMoveAnim, characters[id].uAttackAnim);
		idx++;
	}

	// 動画の描画
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
	// レスポンス解析
	res.ParseResponse(in);
}

void MultiPlayCharacterSelectModeClientSide::Release(RESPONSE_PLAYER &players) {
	for (auto &client : players.clients) {
		// プレイヤーの検索
		auto &player = game_->clients[client.id];

		// 移動属性更新
		{
			ClientAttribute *moveAttribute = CreateAttribute(client.moveAttributeType, player);
			if (player->GetMoveAttribute()) delete player->GetMoveAttribute();
			player->SetMoveAttribute(moveAttribute);
		}
		// 攻撃属性更新
		{
			ClientAttribute *attackAttribute = CreateAttribute(client.attackAttributeType, player);
			if (player->GetAttackAttribute()) delete player->GetAttackAttribute();
			player->SetAttackAttribute(attackAttribute);
		}
	}
}
