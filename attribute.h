#pragma once
#include "lib/vector.h"
#include "player.h"

class Player;
class Attribute : public GameObject
{
protected:
	Player *player_;

public:
	Attribute() = delete;
	Attribute(Player *player) : player_(player) { }

	virtual ~Attribute() = default;

	virtual Vector2 Move() = 0;

	virtual void Action() = 0;

	virtual void Draw(Vector2 offset) = 0;

	virtual void DebugMenu() = 0;

	virtual void Gravity() = 0;
};