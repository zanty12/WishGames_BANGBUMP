#pragma once
#include "multi_mode.h"
#include "lib/math.h"

class MultiPlayCharacterSelectModeServerSide : public MultiPlayServerSide {
private:
	std::list<CLIENT_DATA_SERVER_SIDE> * pCharacters_ = nullptr;



private:
	/// <summary>
	/// 属性入力選択
	/// </summary>
	void AttributeSelect(Vector2 stick, Vector2 previousStick, ATTRIBUTE_TYPE &attributeType) {
		Vector2 direction = stick - previousStick;					// 入力のベクトル
		float accelerationX = direction.x;							// Xの加速度
		const float acceleActivate = 0.3f;							// X入力の判定値
		int type = (int)attributeType;								// 属性

		// X入力に入力されたなら
		if (acceleActivate < MATH::Abs(accelerationX)) {
			// 右に移動
			if (0.0f < accelerationX) type = (type + 1) % 4;
			// 左に移動
			else type = 4 - (type - 1) % 4;
			attributeType = (ATTRIBUTE_TYPE)attributeType;
		}
	}

	void PlayerUpdate(std::list<CLIENT_DATA_SERVER_SIDE> &clients) {
		for (auto client : clients) {
			// 入力情報をセットする
			Input::SetState(0, client.currentInput);
			Input::SetState(0, client.previousInput);

			// 選択する
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
			// キャラクターの設定
			for (auto &character : *pCharacters_)
				res.characters.push_back({ character.header.id, character.moveAttribute, character.actionAttribute });
		}

		// レスポンス作成
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
		// キャラクターが存在しないなら処理しない
		if (res.characters.size() == 0) return;

		const int SCREEN_WIDTH = Graphical::GetWidth();					// 画面の幅
		const int SCREEN_HEIGHT = Graphical::GetHeight();				// 画面の高さ
		const int SCREEN_HORIZONTAL_CENTER = SCREEN_WIDTH * 0.5f;		// 画面の横軸の中心
		const int SCREEN_VERTICAL_CENTER = SCREEN_HEIGHT * 0.5f;		// 画面の縦軸の中心
		const int SCREEN_FRAME_WIDTH = 100;								// 画面の枠の太さ
		const int CHARACTER_FRAME = 50;									// 表示するキャラクター絵の枠の太さ
		const int CHARACTER_DIAGONAL = 100;								// 表示するキャラクター絵の傾き（Xピクセルのズレ）

		int CHARACTER_WIDTH = (SCREEN_WIDTH - SCREEN_FRAME_WIDTH * 2 - CHARACTER_FRAME - CHARACTER_DIAGONAL) / res.characters.size();			// 表示するキャラクター絵の幅
		

		// キャラの描画
		// 画面の枠の部分
		int x = SCREEN_FRAME_WIDTH;										// 描画するX座標
		for (auto character : res.characters) {
			// 表示するキャラクター絵のポリゴン
			Vector2 vertices[] = {
				Vector2(x + CHARACTER_WIDTH + CHARACTER_DIAGONAL,	SCREEN_FRAME_WIDTH),
				Vector2(x + CHARACTER_DIAGONAL,						SCREEN_FRAME_WIDTH),
				Vector2(x + CHARACTER_WIDTH,						SCREEN_HEIGHT - SCREEN_FRAME_WIDTH),
				Vector2(x,											SCREEN_HEIGHT - SCREEN_FRAME_WIDTH),
			};


			float width = CHARACTER_WIDTH;								// 画像の幅
			float height = SCREEN_HEIGHT - 2 * SCREEN_FRAME_WIDTH;		// 画像の高さ
			float aspectRatio = width / height;							// 画像のアスペクト比
			float halfRatio = aspectRatio * 0.5f;						// アスペクト比の半分
			float diagonalRatio = CHARACTER_DIAGONAL / height;			// キャラクター絵の傾きの比率

			// 表示するキャラクター絵のUV値
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
			// 描画
			DrawSprite(0, vertices, uvs, Color::White);

			// 枠の描画
			DrawLine(vertices[0], vertices[1], Color::Black);
			DrawLine(vertices[1], vertices[3], Color::Black);
			DrawLine(vertices[3], vertices[2], Color::Black);
			DrawLine(vertices[2], vertices[0], Color::Black);

			// キャラクター絵の幅の部分
			x += width;

			// キャラクター絵の枠の部分
			x += CHARACTER_FRAME;
		}
	}

	void ParseResponse(Storage &in) override {
		res = RESPONSE_CHARACTER_SELECT();
		// レスポンス解析
		res.ParseResponse(in);
	}

	MULTI_MODE GetMode(void) const override { return CHARACTER_SELECT; }
};
