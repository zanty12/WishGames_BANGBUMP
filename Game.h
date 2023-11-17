#pragma once
#include "Scene.h"
#include "player.h"
#include "mapmngr.h"

class Game :public Scene
{
private:
	Player* player_;
	MapMngr* mapmngr_;

public:

	void Update()override{}
	void Draw()override{}

};
