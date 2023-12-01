#pragma once
#include "scene.h"
#include "player.h"

class Prep :public Scene
{
private:
	Player* player_;

public:

	Prep();
	~Prep();
	void Update()override{}
	void Draw()override{}

};

Prep::Prep()
{
}

Prep::~Prep()
{
}