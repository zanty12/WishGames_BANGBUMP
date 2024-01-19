#pragma once
#include "lib/vector.h"
#include "player.h"
#include "attribute_type.h"


class Player;
class Attribute
{
protected:
	ATTRIBUTE_TYPE attributeType_;
	Player *player_;

public:
	Attribute() = delete;
	Attribute(Player* player, ATTRIBUTE_TYPE attr) : player_(player), attributeType_(attr) {}

	virtual ~Attribute() = default;

	virtual Vector2 Move() = 0;

	virtual void Action() = 0;

	virtual void DebugMenu() = 0;

	Player* GetPlayer() { return player_; }

	ATTRIBUTE_TYPE GetAttribute() { return attributeType_; }
};