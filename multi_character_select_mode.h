#pragma once
#include "multi_mode.h"
#include "lib/math.h"

class MultiPlayCharacterSelectModeServerSide : public MultiPlayServerSide {
private:
	std::list<CLIENT_DATA_SERVER_SIDE> * pCharacters_ = nullptr;

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
	MultiPlayCharacterSelectModeServerSide() : MultiPlayServerSide(nullptr) { };

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
};






class MultiPlayCharacterSelectModeClientSide : public MultiPlayClientSide {
private:
	RESPONSE_CHARACTER_SELECT res;


public:
	MultiPlayCharacterSelectModeClientSide() : MultiPlayClientSide(nullptr) { }

	void Draw(void) override {
		// エリアの描画
		for (auto character : res.characters) {
			//anim.SetTexNo(areaTexNo);
			//anim.SetPos(area.position);
			//anim.SetScale(Vector2(area.radius, area.radius));
			//anim.Draw();
		}
	}

	void ParseResponse(Storage &in) override {
		res = RESPONSE_CHARACTER_SELECT();
		// レスポンス解析
		res.ParseResponse(in);
	}
};
