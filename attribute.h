#pragma once
#include "lib/vector.h"
#include "player.h"
#include "attribute_type.h"


enum ATTRIBUTE
{
	ATTR_FIRE = 1,
	ATTR_DARK,
	ATTR_WIND,
	ATTR_THUNDER,
};

class Player;
class Attribute
{
protected:
	ATTRIBUTE_TYPE attributeType_;
	Player *player_;
	ATTRIBUTE attr_;

public:
	Attribute() = delete;
	Attribute(Player* player, ATTRIBUTE attr) : player_(player), attr_(attr) {}

	virtual ~Attribute() = default;

	virtual Vector2 Move() = 0;

	virtual void Action() = 0;

	virtual void Draw(Vector2 offset) = 0;

	virtual void DebugMenu() = 0;

	virtual void Gravity() = 0;

	ATTRIBUTE GetAttribute() { return attr_; }
};