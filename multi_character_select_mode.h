#pragma once
#include <map>
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
	/// ëÆê´ì¸óÕëIë
	/// </summary>
	void AttributeSelect(Vector2 stick, Vector2 previousStick, ATTRIBUTE_TYPE &attributeType);

	void PlayerUpdate(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients);

	/// <summary>
	/// ëÆê´çÏê¨
	/// </summary>
	ServerAttribute *CreateAttribute(ATTRIBUTE_TYPE type, ServerPlayer * player);

public:
	MultiPlayCharacterSelectModeServerSide(MultiPlayServer *game)
		: MultiPlayModeServerSide(new MultiMap(MAP_PATH + "MultiPlay_Map0.csv", MULTIPLAY_RUN_TYPE_SERVER), L"CharacterSelect"), game_(game) {
	}

	void Release(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) override;

	void Update(std::map<int, CLIENT_DATA_SERVER_SIDE> &clients) override;

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
	/// ëÆê´çÏê¨
	/// </summary>
	ClientAttribute *CreateAttribute(ATTRIBUTE_TYPE type, ClientPlayer *player);

private:
	RESPONSE_CHARACTER_SELECT res;
	std::map<int, AnimData> characters;
	int charsTexNo = LoadTexture("data/texture/player1_11_22_33_44.png");
	MultiPlayClient *game_ = nullptr;
	Video *video = nullptr;

private:
	void CharacterDraw(int idx, int maxIdx, float protrude, float gap, float showAttribute, float showRateMin, float showRateMax);

public:
	MultiPlayCharacterSelectModeClientSide(MultiPlayClient * game) : MultiPlayModeClientSide(new MultiMap(MAP_PATH + "MultiPlay_Map0.csv", MULTIPLAY_RUN_TYPE_CLIENT), L"CharacterSelect"), game_(game) {
		map_->backBGTexNo = LoadTexture(Asset::textures_.at(textures::bg_stage1_back));
		map_->frontBGTexNo = LoadTexture(Asset::textures_.at(textures::bg_stage1_back));
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

	void Draw(RESPONSE_PLAYER &players, Vector2 offset) override;
	void ParseResponse(Storage &in) override;
	void Release(RESPONSE_PLAYER &players) override;

	MULTI_MODE GetMode(void) const override { return CHARACTER_SELECT; }
};
