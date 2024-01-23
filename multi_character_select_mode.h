#pragma once
#include <map>
#include "multi_mode.h"
#include "lib/math.h"
#include "asset.h"
#include "prep.h"
#include "follow.h"

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
		: MultiPlayModeServerSide(new MultiMap("data/map/MultiPlay_Map0.csv")), game_(game) {
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

private:
	RESPONSE_CHARACTER_SELECT res;
	std::map<int, AnimData> characters;
	int charsTexNo = LoadTexture("data/texture/player1_11_22_33_44.png");
	//Prep prep = nullptr;
	MultiPlayClient *game_ = nullptr;


private:
	void CharacterDraw(int idx, int maxIdx, float protrude, float gap, float showAttribute, float showRateMin, float showRateMax);

public:
	MultiPlayCharacterSelectModeClientSide(MultiPlayClient * game) : MultiPlayModeClientSide(new MultiMap("data/map/MultiPlay_Map0.csv")), game_(game) {
		map_->backBGTexNo = LoadTexture(Asset::textures_.at(textures::bg_stage1_back));
		map_->frontBGTexNo = LoadTexture(Asset::textures_.at(textures::bg_stage1_back));
	}

	void Draw(RESPONSE_PLAYER &players, Vector2 offset) override;

	void ParseResponse(Storage &in) override;

	MULTI_MODE GetMode(void) const override { return CHARACTER_SELECT; }
};
