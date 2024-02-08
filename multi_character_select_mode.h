#pragma once
#include <map>
#include "multiplay.h"
#include "multi_mode.h"
#include "multi_runenum.h"
#include "lib/math.h"
#include "asset.h"
#include "prep.h"
#include "follow.h"
#include "video.h"

class MultiPlayServer;
class MultiPlayClient;
/***********************************************************
	Server
************************************************************/
class MultiPlayCharacterSelectModeServerSide : public MultiPlayModeServerSide {
private:
	MultiPlayServer *game_ = nullptr;

private:
	/// <summary>
	/// 属性入力選択
	/// </summary>
	void AttributeSelect(Vector2 stick, Vector2 previousStick, ATTRIBUTE_TYPE &attributeType);

	void PlayerUpdate(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients);

	/// <summary>
	/// 属性作成
	/// </summary>
	ServerAttribute *CreateAttribute(ATTRIBUTE_TYPE type, ServerPlayer * player);

public:
	MultiPlayCharacterSelectModeServerSide(MultiPlayServer *game)
		: MultiPlayModeServerSide(L"CharacterSelect"), game_(game) {
	}

	void Release(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) override;

	void Update(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) override;
	void UpdateResult(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) override;
	void CreateResponse(Storage &out) override;

	bool IsPlayerMove(void) const override { return false; }
	MULTI_MODE GetMode(void) const override { return CHARACTER_SELECT; }
};






/***********************************************************
	Client
************************************************************/
class MultiPlayCharacterSelectModeClientSide : public MultiPlayModeClientSide {
private:
	struct AnimData {
		Follow uAttackAnim, uMoveAnim;
		ATTRIBUTE_TYPE attackAttributeType = ATTRIBUTE_TYPE_FIRE, moveAttributeType = ATTRIBUTE_TYPE_FIRE;

		AnimData() = default;
		AnimData(ATTRIBUTE_TYPE attackAttributeType, ATTRIBUTE_TYPE moveAttributeType)
			: attackAttributeType(attackAttributeType), moveAttributeType(moveAttributeType) { }

		void set(ATTRIBUTE_TYPE attackAttributeType, ATTRIBUTE_TYPE moveAttributeType) {
			this->attackAttributeType = attackAttributeType, this->moveAttributeType = moveAttributeType;
		}
	};

	/// <summary>
	/// 属性作成
	/// </summary>
	ClientAttribute *CreateAttribute(ATTRIBUTE_TYPE type, ClientPlayer *player);

private:
	RESPONSE_CHARACTER_SELECT res;
	std::map<int, AnimData> characters;
	int charsTexNo = LoadTexture("data/texture/player1_11_22_33_44.png");
	MultiPlayClient *game_ = nullptr;
	Video *video = nullptr;
	int bootTexNo[ATTRIBUTE_TYPE_NUM] = {};
	int handTexNo[ATTRIBUTE_TYPE_NUM] = {};
	int playerTexNo[4] = {};

private:
	void CharacterDraw(int id, int maxIdx, float width, float height, float gap, int moveAttribute, int attackAttribute, float moveAttributeSmooth, float attackAttributeSmooth);

public:
	MultiPlayCharacterSelectModeClientSide(MultiPlayClient * game) : MultiPlayModeClientSide(L"CharacterSelect"), game_(game) {
		map_->backBGTexNo = LoadTexture(Asset::textures_.at(textures::bg_stage1_back));
		map_->frontBGTexNo = LoadTexture(Asset::textures_.at(textures::bg_stage1_back));

		// プレイヤーテクスチャの取得
		bootTexNo[0] = LoadTexture("data/texture/UI/player_boot1.png");
		bootTexNo[1] = LoadTexture("data/texture/UI/player_boot2.png");
		bootTexNo[2] = LoadTexture("data/texture/UI/player_boot3.png");
		bootTexNo[3] = LoadTexture("data/texture/UI/player_boot4.png");
		handTexNo[0] = LoadTexture("data/texture/UI/player_hand1.png");
		handTexNo[1] = LoadTexture("data/texture/UI/player_hand2.png");
		handTexNo[2] = LoadTexture("data/texture/UI/player_hand3.png");
		handTexNo[3] = LoadTexture("data/texture/UI/player_hand4.png");
		playerTexNo[0] = LoadTexture("data/texture/UI/player1_base.png");
		playerTexNo[1] = LoadTexture("data/texture/UI/player2_base.png");
		playerTexNo[2] = LoadTexture("data/texture/UI/player3_base.png");
		playerTexNo[3] = LoadTexture("data/texture/UI/player4_base.png");

		soNo = LoadSound("data/sound/BGM/select_element_BGM.wav");



		video = new Video("./data/video/fire_move.mp4");

		video->SetSize(Vector2(853.3 * static_cast<float>(Graphical::GetWidth()) / 1920,
			480 * static_cast<float>(Graphical::GetHeight()) / 1080));
		video->SetLoop(true);
		video->SetWindowPos(Vector2(1400 * static_cast<float>(Graphical::GetWidth()) / 1920,
			1000 / 2 * static_cast<float>(Graphical::GetHeight()) / 1080));
	}

	~MultiPlayCharacterSelectModeClientSide() {
		delete video;
	}

	void DrawStart(RESPONSE_PLAYER &players, Vector2 offset) override;
	void DrawResult(RESPONSE_PLAYER &players, Vector2 offset) override;
	void Draw(RESPONSE_PLAYER &players, Vector2 offset) override;
	void ParseResponse(Storage &in) override;
	void Release(RESPONSE_PLAYER &players) override;

	MULTI_MODE GetMode(void) const override { return CHARACTER_SELECT; }
};
