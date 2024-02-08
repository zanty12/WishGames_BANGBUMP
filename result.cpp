#include "result.h"
#include "xinput.h"

static int	bg_tex_ = 0;

Result::Result(SceneMngr* scene_mngr)
	:scene_mngr_(scene_mngr)
{
	bg_tex_ = LoadTexture((char*)"data/TEXTURE/result.webp");
}

Result::~Result()
{
}

void Result::Update()
{
	//if (Input::GetKeyDown(0, Input::North))	//A�{�^��
	//{
	//	scene_mngr_->ChangeScene(SCENE_TITLE);
	//}
}

void Result::Draw()
{
	DrawSprite(bg_tex_, Vector2(960.0f, 540.0f), 0.0f, Vector2(1920.0f, 1080.0f), Color(1.0f, 1.0f, 1.0f, 1.0f));
}