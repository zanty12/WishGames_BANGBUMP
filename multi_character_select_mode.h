#pragma once
#include <map>
#include "multi_mode.h"
#include "prep.h"
#include "lib/math.h"
#include "follow.h"

class MultiPlayServer;
class MultiPlayClient;
/***********************************************************
	Server
************************************************************/
class MultiPlayCharacterSelectModeServerSide : public MultiPlayServerSide {
private:
	std::list<CLIENT_DATA_SERVER_SIDE> * pCharacters_ = nullptr;
	MultiPlayServer *game_ = nullptr;

private:
	/// <summary>
	/// ëÆê´ì¸óÕëIë
	/// </summary>
	void AttributeSelect(Vector2 stick, Vector2 previousStick, ATTRIBUTE_TYPE &attributeType);

	void PlayerUpdate(std::list<CLIENT_DATA_SERVER_SIDE> &clients);

	/// <summary>
	/// ëÆê´çÏê¨
	/// </summary>
	Attribute *CreateAttribute(ATTRIBUTE_TYPE type, Player* player);

public:
	MultiPlayCharacterSelectModeServerSide(MultiPlayServer *game)
		: MultiPlayServerSide(nullptr) , game_(game) {
		maxTime_ = 10;
	}

	void Release(std::list<CLIENT_DATA_SERVER_SIDE> &clients) override;

	void Update(std::list<CLIENT_DATA_SERVER_SIDE> &clients) override;

	void CreateResponse(Storage &out) override;

	MULTI_MODE GetMode(void) const override { return CHARACTER_SELECT; }
};






/***********************************************************
	Client
************************************************************/
class MultiPlayCharacterSelectModeClientSide : public MultiPlayClientSide {
private:
	//struct AnimData {
	//	Follow uAttackAnim, uMoveAnim;
	//	ATTRIBUTE_TYPE attackAttributeType = ATTRIBUTE_TYPE_FIRE, moveAttributeType = ATTRIBUTE_TYPE_FIRE;

	//	AnimData() = default;
	//	AnimData(ATTRIBUTE_TYPE attackAttributeType, ATTRIBUTE_TYPE moveAttributeType)
	//		: attackAttributeType(attackAttributeType), moveAttributeType(moveAttributeType) { }

	//	void set(ATTRIBUTE_TYPE attackAttributeType, ATTRIBUTE_TYPE moveAttributeType) {
	//		this->attackAttributeType = attackAttributeType, this->moveAttributeType = moveAttributeType;
	//	}
	//};

private:
	RESPONSE_CHARACTER_SELECT res;
	//std::map<int, AnimData> characters;
	//int charsTexNo = LoadTexture("data/texture/player1_11_22_33_44.png");
	Prep prep = nullptr;
	MultiPlayClient *game_ = nullptr;


private:
	void CharacterDraw(int idx, int maxIdx, float protrude, float gap, float showAttribute, float showRateMin, float showRateMax);

public:
	MultiPlayCharacterSelectModeClientSide(MultiPlayClient * game) : MultiPlayClientSide(nullptr), game_(game) { }

	void Draw(RESPONSE_PLAYER &players) override;

	void ParseResponse(Storage &in) override;

	MULTI_MODE GetMode(void) const override { return CHARACTER_SELECT; }
};
