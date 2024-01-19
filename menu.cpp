#include "menu.h"
#include "xinput.h"

static int	g_TextureNo = 0;

Menu::Menu(SceneMngr* scene_mngr)
	:scene_mngr_(scene_mngr)
{
	g_TextureNo = LoadTexture((char*)"data/TEXTURE/menu.png");
}

Menu::~Menu()
{
}

void Menu::Update()
{

	if (Input::GetStickLeft(0).x > 0.0f) //マルチ
	{
		scene_mngr_->ChangeScene(SCENE_PREP);
	}
	//コントローラーを接続しないとずっとタイトルに行くよ(^^♪
	else if(Input::GetStickLeft(0).x < 0.0f) //ソロ
	{
		scene_mngr_->ChangeScene(SCENE_PREP);
	}

}

void Menu::Draw()
{
	DrawSprite(g_TextureNo, Vector2(830.0f, 467.0f), 0.0f, Vector2(1920.0f, 1080.0f), Color(1.0f, 1.0f, 1.0f, 1.0f));
}

void Menu::DebugMenu()
{
}
