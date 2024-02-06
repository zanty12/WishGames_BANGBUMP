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

void MultiPlayCharacterSelectModeClientSide::CharacterDraw(int idx, int maxIdx, float protrude, float gap, float showAttribute, float showRateMin, float showRateMax) {
	const float SCREEN_WIDTH = Graphical::GetWidth();				// 画面の幅
	const float SCREEN_HEIGHT = Graphical::GetHeight();				// 画面の高さ

	// 実際に使える幅 / maxIdx - キャラ分の隙間
	//float width = (SCREEN_WIDTH) / maxIdx - (gap * (maxIdx - 1) * 0.5f) - gap;	// 立ち絵の幅（隙間を考慮）
	float width = (SCREEN_WIDTH - maxIdx * gap - gap - protrude) / maxIdx;	// 立ち絵の幅（隙間を考慮）
	float height = SCREEN_HEIGHT - 2 * gap;							// 立ち絵の高さ



	// キャラの描画
	// 画面の枠の部分
	float x = gap + (width + gap) * idx;							// 描画するX座標

	// 上半身 or 下半身のみを描画したいから
	// X座標どこで区切るのか計算する
	float minHorizontal = MATH::Leap(x + protrude, x, showRateMin);
	float maxHorizontal = MATH::Leap(x + protrude, x, showRateMax);
	// Y座標どこからどこまでを描画するか計算する
	float minVertical = MATH::Leap(gap, SCREEN_HEIGHT - gap, showRateMin);
	float maxVertical = MATH::Leap(gap, SCREEN_HEIGHT - gap, showRateMax);

	// 表示するキャラクター絵のポリゴン
	Vector2 vertices[] = {
		Vector2(minHorizontal + width,	minVertical),	// 右上
		Vector2(minHorizontal,			minVertical),	// 左上
		Vector2(maxHorizontal + width,	maxVertical),	// 右下
		Vector2(maxHorizontal,			maxVertical),	// 左下
	};


	float aspectRatio = width / height;								// 画像のアスペクト比
	float halfRatio = aspectRatio * 0.5f;							// アスペクト比の半分
	float diagonalRatio = protrude / height;						// キャラクター絵の傾きの比率
	float uCenter = showAttribute * 0.25f + 0.125;					// 表示したいキャラ
	// 4キャラ一気に表示されるため
	halfRatio /= 4.0f;
	diagonalRatio /= 4.0f;


	// U座標どこで区切るのか計算する
	float minU = MATH::Leap(diagonalRatio * 0.5f, -diagonalRatio * 0.5f, showRateMin);
	float maxU = MATH::Leap(diagonalRatio * 0.5f, -diagonalRatio * 0.5f, showRateMax);
	// 表示するキャラクター絵のUV値
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
	// 描画
	DrawSprite(LoadTexture("data/texture/player1_11_22_33_44.png"), vertices, uvs, Color::White);

	// 枠の描画
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
		CharacterDraw(idx, res.characters.size(), 0, 10, characters[id].uAttackAnim, 0.0f, 0.6f);
		CharacterDraw(idx, res.characters.size(), 0, 10, characters[id].uMoveAnim, 0.6f, 1.0f);
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
