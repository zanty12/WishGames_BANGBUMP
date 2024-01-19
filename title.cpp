#include "title.h"
#include "xinput.h"
#include "asset.h"
#include "sound.h"

static int	g_TextureNo = 0;
static int g_BGM = 0;

Title::Title(SceneMngr* scene_mngr)
	:scene_mngr_(scene_mngr)
{
	g_TextureNo = LoadTexture((char*)"data/TEXTURE/title.webp");
	g_BGM = LoadSound(Asset::GetAsset(SE_1).c_str());
	PlaySound(g_BGM, -1);
}

Title::~Title()
{
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
	DrawSprite(g_TextureNo, Vector2(960.0f, 540.0f), 0.0f, Vector2(1920.0f, 1080.0f), Color(1.0f, 1.0f, 1.0f, 1.0f));
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