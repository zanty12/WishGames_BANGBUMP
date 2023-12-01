#include "result.h"
#include "xinput.h"

static int	g_TextureNo = 0;

Result::Result(SceneMngr* scene_mngr)
	:scene_mngr_(scene_mngr)
{
	g_TextureNo = LoadTexture((char*)"data/TEXTURE/result.webp");
}

Result::~Result()
{
}

void Result::Update()
{
	if (Input::GetKeyDown(0, Input::North))	//Aƒ{ƒ^ƒ“
	{
		scene_mngr_->ChangeScene(SCENE_TITLE);
	}
}

void Result::Draw()
{
	DrawSprite(g_TextureNo, Vector2(960.0f, 540.0f), 0.0f, Vector2(1920.0f, 1080.0f), Color(1.0f, 1.0f, 1.0f, 1.0f));
}