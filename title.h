#pragma once
#include "sprite.h"
#include "scene.h"
#include "scenemngr.h"

class Title :public Scene
{
private:
	SceneMngr* scene_mngr_;
public:
	Title(SceneMngr* scene_mngr);
	~Title();
	void Update()override;
	void Draw()override;
	void DebugMenu() override;
};

