#pragma once
#include "sprite.h"
#include "scene.h"
#include "scenemngr.h"

class Result :public Scene
{
private:
	SceneMngr* scene_mngr_;
public:
	Result(SceneMngr* scene_mngr);
	~Result();
	void Update()override;
	void Draw()override;
	void DebugMenu() override {}
};

