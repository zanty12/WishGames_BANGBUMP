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
/// 属性入力選択
/// </summary>
void MultiPlayCharacterSelectModeServerSide::AttributeSelect(Vector2 stick, Vector2 previousStick, ATTRIBUTE_TYPE &attributeType) {
	Vector2 direction = stick - previousStick;					// 入力のベクトル
	float accelerationX = direction.x;							// Xの加速度
	const float acceleActivate = 0.01f;							// X入力の判定値
	int type = (int)attributeType;								// 属性

	// X入力に入力されたなら
	if (stick != Vector2::Zero) {
		int   i = 0;
	}
	if (acceleActivate < MATH::Abs(accelerationX)) {
		// 右に移動
		if (0.0f < accelerationX) type = ((type + 1) % 4);
		// 左に移動
		else type = (4 - ((type - 1) % 4));
		// 万が一属性がエラー値になった場合初期化する
		if (type < 0 || 4 < type) type = 0;
		attributeType = (ATTRIBUTE_TYPE)type;
	}
}

void MultiPlayCharacterSelectModeServerSide::PlayerUpdate(std::list<CLIENT_DATA_SERVER_SIDE> &clients) {
	for (auto &client : clients) {
		// 入力情報をセットする
		Input::SetState(0, client.currentInput);
		Input::SetPreviousState(0, client.previousInput);

		// 選択する
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
		// プレイヤーの検索
		auto iterator = game_->find(client.header.id);

		// IDが存在しないなら処理しない
		if (iterator == game_->GetClients().end()) continue;

		// 移動属性更新
		{
			Attribute *moveAttribute = CreateAttribute(iterator->moveAttribute, iterator->player_);
			iterator->player_->SetAttribute(moveAttribute);
		}
		// 攻撃属性更新
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
		// キャラクターの設定
		for (auto &character : *pCharacters_)
			res.characters.push_back({ character.header.id, character.moveAttribute, character.actionAttribute });
	}

	// レスポンス作成
	res.CreateResponse(out);
}






/***********************************************************
	Client
************************************************************/
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

void MultiPlayCharacterSelectModeClientSide::Draw(RESPONSE_PLAYER &players) {
	DebugUI::BeginDraw();
	Text::TextStart();
	prep.Update();
	prep.Draw();
	game_->move_ = (ATTRIBUTE_TYPE)((int)prep.GetMove() * 0.5f);
	game_->action_ = (ATTRIBUTE_TYPE)((int)prep.GetAttack() * 0.5f);
	Text::TextEnd();
	DebugUI::EndDraw();
	//// キャラクターが存在しないなら処理しない
	//if (players.clients.size() == 0) return;

	//int idx = 0;								// インデックス
	//int playerNum = players.clients.size();		// プレイヤー数
	//int protrude = 100;							// 傾き
	//int gap = 50;								// 枠の幅
	//for (auto &c : players.clients) {
	//	auto iterator = characters.find(c.id);
	//	// 登録
	//	if (iterator == characters.end())
	//		characters[c.id] = AnimData(c.attackAttributeType, c.moveAttributeType);
	//	// 更新
	//	else iterator->second.set(c.attackAttributeType, c.moveAttributeType);

	//	// パラメータの更新
	//	auto &anim = characters[c.id];
	//	auto &uAttack = anim.uAttackAnim;
	//	auto &uMove = anim.uMoveAnim;
	//	uAttack = c.attackAttributeType;
	//	uMove = c.moveAttributeType;
	//	uAttack.update();
	//	uMove.update();

	//	CharacterDraw(idx, playerNum, protrude, gap, uAttack, 0.0f, 0.5f);
	//	CharacterDraw(idx, playerNum, protrude, gap, uMove, 0.5f, 1.0f);

	//	// インデックスを増やす
	//	idx++;
	//}
}

void MultiPlayCharacterSelectModeClientSide::ParseResponse(Storage &in) {
	res = RESPONSE_CHARACTER_SELECT();
	// レスポンス解析
	res.ParseResponse(in);
}