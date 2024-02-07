#pragma once
#include "sprite.h"
#include "scene.h"
#include "scenemngr.h"
#include "video.h"

class Title :public Scene
{
private:
	SceneMngr* scene_mngr_;
	Video* title_video_;
	int logo_tex_;
	bool logo_ = true;
	float logo_time_ = 3.0f;
	float logo_alpha_ = 0.0f;
public:
	Title(SceneMngr* scene_mngr);
	~Title();
	void Update()override;
	void Draw()override;

	void DebugMenu() override;
};

