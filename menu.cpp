#include "menu.h"
#include "xinput.h"

Menu::Menu(SceneMngr* scene_mngr)
    : scene_mngr_(scene_mngr)
{
    bg_tex_ = LoadTexture((char*)"data/TEXTURE/UI/menu.png");
    multi_tex_ = LoadTexture((char*)"data/TEXTURE/UI/online_mode.png");
    solo_tex_ = LoadTexture((char*)"data/TEXTURE/UI/single_mode.png");
    multi_ = true;
    lerp_timer_.Start();
}

Menu::~Menu()
{
}

void Menu::Update()
{
    if (Input::GetStickLeft(0).x > 0.2f && multi_) //solo
    {
        multi_ = false;
        lerp_timer_.Start();
    }
    //コントローラーを接続しないとずっとタイトルに行くよ(^^♪
    else if (Input::GetStickLeft(0).x < -0.2f && !multi_) //multi
    {
        multi_ = true;
        lerp_timer_.Start();
    }

    if (Input::GetKeyDown(0, Input::A))
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
    float lerp_percent = lerp_timer_.GetNowTime() * 0.001f / 0.5f;
    const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
    const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
    DrawSprite(bg_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 2), 0.0f,
               Vector2(1920.0f * scale_x, 1080.0f * scale_y), Color(1.0f, 1.0f, 1.0f, 1.0f));
    if (multi_)
    {
        DrawSprite(multi_tex_, Vector2(Graphical::GetWidth() / 4, Graphical::GetHeight() / 2), 0.0f,
                   Vector2(650.0f * scale_x, 650.0f * scale_y) * LerpScale(1.0f,1.23,lerp_percent), Color(1.0f, 1.0f, 1.0f, 1.0f));
        DrawSprite(solo_tex_, Vector2(Graphical::GetWidth() / 4 * 3, Graphical::GetHeight() / 2), 0.0f,
                   Vector2(650.0f * scale_x, 650.0f * scale_y), Color(0.5f, 0.5f, 0.5f, 1.0f));
    }
    else
    {
        DrawSprite(multi_tex_, Vector2(Graphical::GetWidth() / 4, Graphical::GetHeight() / 2), 0.0f,
                   Vector2(650.0f * scale_x, 650.0f * scale_y), Color(0.5f, 0.5f, 0.5f, 1.0f));
        DrawSprite(solo_tex_, Vector2(Graphical::GetWidth() / 4 * 3, Graphical::GetHeight() / 2), 0.0f,
                   Vector2(650.0f * scale_x, 650.0f * scale_y)* LerpScale(1.0f,1.23,lerp_percent), Color(1.0f, 1.0f, 1.0f, 1.0f));
    }
}

void Menu::DebugMenu()
{
}

float Menu::LerpScale(float start, float end, float percent)
{
    if(percent > 1.0f)
    {
        percent = 1.0f;
    }
    return (start + percent * (end - start));
}
