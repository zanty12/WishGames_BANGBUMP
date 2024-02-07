#pragma once
#include "sprite.h"
#include "scene.h"
#include "scenemngr.h"

class Menu :public Scene
{
private:
	SceneMngr* scene_mngr_;
	int bg_tex_;
	int multi_tex_;
	int solo_tex_;
	bool multi_;
public:
	Menu(SceneMngr* scene_mngr);
	~Menu() override;
	void Update()override;
	void Draw()override;
	void DebugMenu() override;
};