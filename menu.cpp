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

	if (Input::GetStickLeft(0).x > 0.0f) //ƒ}ƒ‹ƒ`
	{
		scene_mngr_->ChangeScene(SCENE_MULTI_SERVER_SELECT);
	}
	
	if(Input::GetStickLeft(0).x < 0.0f) //ƒ\ƒ
	{
		scene_mngr_->ChangeScene(SCENE_PREP);
	}

}

void Menu::Draw()
{
	const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
	const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
	DrawSprite(g_TextureNo, Vector2(Graphical::GetWidth()/2, Graphical::GetHeight()/2), 0.0f, Vector2(1920.0f*scale_x, 1080.0f*scale_y), Color(1.0f, 1.0f, 1.0f, 1.0f));
}

void Menu::DebugMenu()
{
}
