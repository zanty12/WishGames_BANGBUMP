#pragma once
#include "sprite.h"
#include "scene.h"
#include "scenemngr.h"
#include "video.h"
#include "lib/win_time.h"

class Menu :public Scene
{
private:
	SceneMngr* scene_mngr_;
	int bg_tex_;
	int multi_tex_;
	int solo_tex_;
	int text_tex_;
	int scene_title_tex_;
	int select_tex_;
	int confirm_tex_;
	int return_tex_;
	int bgm_;
	int select_se_;
	int confirm_se_;
	bool multi_;
	WIN::Time lerp_timer_;
	Video* bg_video_ = nullptr;

public:
	Menu(SceneMngr* scene_mngr);
	~Menu() override;
	void Update()override;
	void Draw()override;
	void DebugMenu() override;
private:
	float LerpScale(float start, float end, float percent);
};
