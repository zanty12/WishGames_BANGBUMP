#include "title.h"
#include "xinput.h"
#include "asset.h"
#include "sound.h"

static int	bg_tex_ = 0;
static int g_BGM = 0;

Title::Title(SceneMngr* scene_mngr)
	:scene_mngr_(scene_mngr)
{
	bg_tex_ = LoadTexture((char*)"data/TEXTURE/title.webp");
	g_BGM = LoadSound(Asset::GetAsset(SE_1).c_str());
	PlaySound(g_BGM, -1);
}

Title::~Title()
{
	StopSoundAll();
}

void Title::Update()
{
	if (Input::GetKeyDown(0, Input::North))	//Aボタン
	{
		scene_mngr_->ChangeScene(SCENE_MENU);
	}
}

void Title::Draw()
{
	const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
	const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
	DrawSprite(bg_tex_, Vector2(Graphical::GetWidth()/2, Graphical::GetHeight()/2), 0.0f, Vector2(1920.0f*scale_x, 1080.0f*scale_y), Color(1.0f, 1.0f, 1.0f, 1.0f));
}

void Title::DebugMenu()
{
	ImGui::Begin(u8"タイトル");
	if(ImGui::Button(u8"ゲームスタート"))
	{
		scene_mngr_->ChangeScene(SCENE_MENU);
	}
	ImGui::End();
}