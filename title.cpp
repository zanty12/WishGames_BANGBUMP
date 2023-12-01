#include "title.h"
#include "xinput.h"
#include "scenemngr.h"
static int	g_TextureNo = 0;

Title::Title()
{
	g_TextureNo = LoadTexture((char*)"data/TEXTURE/title.png");
}

Title::~Title()
{
}

void Title::Update()
{
	if (Input::GetKey(0, Input::LThumb)) {
		

	}
}

void Title::Draw()
{
	DrawSprite(g_TextureNo, Vector2(960.0f, 540.0f), 0.0f, Vector2(1920.0f, 1080.0f), Color(1.0f, 1.0f, 1.0f, 1.0f));
}