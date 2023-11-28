#pragma once
#include"attribute.h"

class Fire :public Attribute
{

public:
	Fire(Player& player) : Attribute(player) { }
	Vector2 Move()override;
	void Action()override;
};