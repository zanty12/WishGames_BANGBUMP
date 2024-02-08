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
	int press_button_tex_;
	int title_tex_;
	int bgm_;
	int confirm_se_;
	//control
	bool logo_ = true;
	bool bgm_start_ = false;
	bool game_start_ = false;
	float logo_time_ = 3.0f;
	float logo_alpha_ = 0.0f;
	float fade_alpha_ = 1.0f;
	float video_fade_ = 0.0f;
	float title_wait = 2.0f;
	float title_scale_start_ = 10.0f;
	float title_scale_ = 10.0f;
	float game_start_wait_ = 1.0f;


public:
	Title(SceneMngr* scene_mngr);
	~Title();
	void Update()override;
	void Draw()override;

	void DebugMenu() override;

private:
	float AlphaAnimation(float rate = 1.0f);
};

