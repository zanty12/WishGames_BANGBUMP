#include "menu.h"
#include "xinput.h"

Menu::Menu(SceneMngr* scene_mngr)
    : scene_mngr_(scene_mngr)
{
    bg_tex_ = LoadTexture((char*)"data/TEXTURE/UI/menu.png");
    multi_tex_ = LoadTexture((char*)"data/TEXTURE/UI/online_mode.png");
    solo_tex_ = LoadTexture((char*)"data/TEXTURE/UI/single_mode.png");
    multi_ = true;
}

Menu::~Menu()
{
}

void Menu::Update()
{
    if (Input::GetStickLeft(0).x > 0.0f) //マルチ
    {
        multi_ = false;
    }
    //コントローラーを接続しないとずっとタイトルに行くよ(^^♪
    else if (Input::GetStickLeft(0).x < 0.0f) //ソロ
    {
        multi_ = true;
    }

    if (Input::GetKeyUp(0, Input::A))
    {
        if (multi_)
        {
            scene_mngr_->ChangeScene(SCENE_MULTI_SERVER_SELECT);
        }
        else
        {
            scene_mngr_->ChangeScene(SCENE_PREP);
        }
    }
}

void Menu::Draw()
{
    const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
    const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
    DrawSprite(bg_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 2), 0.0f,
               Vector2(1920.0f * scale_x, 1080.0f * scale_y), Color(1.0f, 1.0f, 1.0f, 1.0f));
    if (multi_)
    {
        DrawSprite(multi_tex_, Vector2(Graphical::GetWidth() / 4, Graphical::GetHeight() / 2.5), 0.0f,
                   Vector2(800.0f * scale_x, 800.0f * scale_y), Color(1.0f, 1.0f, 1.0f, 1.0f));
        DrawSprite(solo_tex_, Vector2(Graphical::GetWidth() / 4 * 3, Graphical::GetHeight() / 2.5), 0.0f,
                   Vector2(750.0f * scale_x, 750.0f * scale_y), Color(1.0f, 1.0f, 1.0f, 1.0f));
    }
    else
    {
        DrawSprite(multi_tex_, Vector2(Graphical::GetWidth() / 4, Graphical::GetHeight() / 2.5), 0.0f,
                   Vector2(750.0f * scale_x, 750.0f * scale_y), Color(1.0f, 1.0f, 1.0f, 1.0f));
        DrawSprite(solo_tex_, Vector2(Graphical::GetWidth() / 4 * 3, Graphical::GetHeight() / 2.5), 0.0f,
                   Vector2(800.0f * scale_x, 800.0f * scale_y), Color(1.0f, 1.0f, 1.0f, 1.0f));
    }
}

void Menu::DebugMenu()
{
}
