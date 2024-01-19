#pragma once
#include "sprite.h"
#include "scene.h"
#include "scenemngr.h"

class Menu :public Scene
{
private:
	SceneMngr* scene_mngr_;
public:
	Menu(SceneMngr* scene_mngr);
	~Menu();
	void Update()override;
	void Draw()override;
	void DebugMenu() override;
};