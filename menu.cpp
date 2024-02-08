#include "menu.h"

#include "sound.h"
#include "xinput.h"

Menu::Menu(SceneMngr* scene_mngr)
    : scene_mngr_(scene_mngr)
{
    //texture
    multi_tex_ = LoadTexture((char*)"data/TEXTURE/UI/menu/online_mode.png");
    solo_tex_ = LoadTexture((char*)"data/TEXTURE/UI/menu/single_mode.png");
    text_tex_ = LoadTexture((char*)"data/TEXTURE/UI/menu/mode_text.png");
    scene_title_tex_ = LoadTexture((char*)"data/TEXTURE/UI/menu/UI_menu.png");
    select_tex_ = LoadTexture((char*)"data/TEXTURE/UI/select.png");
    stick_tex_ = LoadTexture((char*)"data/TEXTURE/UI/stick.png");
    confirm_tex_ =  LoadTexture((char*)"data/TEXTURE/UI/go.png");
    a_tex_ = LoadTexture((char*)"data/TEXTURE/UI/A.png");
    return_tex_ = LoadTexture((char*)"data/TEXTURE/UI/back.png");
    b_tex_ = LoadTexture((char*)"data/TEXTURE/UI/B.png");
    //sound
    bgm_ = LoadSound("data/SOUND/bgm/modeselect.wav");
    select_se_ = LoadSound((char*)"data/SOUND/se/modeselect_SE.wav");
    confirm_se_ = LoadSound((char*)"data/SOUND/se/confirm.wav");

    bg_video_ = new Video("data/video/BG_modeselect.mp4");
    bg_video_->SetLoop(true);
    bg_video_->SetWindowPos(Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 2));
    bg_video_->SetSize(Vector2(Graphical::GetWidth(), Graphical::GetHeight()));

    multi_ = true;
    lerp_timer_.Start();
}

Menu::~Menu()
{
    delete bg_video_;
    StopSound(bgm_);
}

void Menu::Update()
{
    if(first_update_)
    {
        PlaySound(bgm_, -1);
        SetVolume(bgm_, 0.4f);
        first_update_ = false;
    }
    bg_video_->Update();
    if (Input::GetStickLeft(0).x > 0.2f && multi_) //solo
    {
        multi_ = false;
        lerp_timer_.Start();
        PlaySound(select_se_, 0);
    }
    //コントローラーを接続しないとずっとタイトルに行くよ(^^♪
    else if (Input::GetStickLeft(0).x < -0.2f && !multi_) //multi
    {
        multi_ = true;
        lerp_timer_.Start();
        PlaySound(select_se_, 0);
    }

    if (Input::GetKeyDown(0, Input::A))
    {
        PlaySound(confirm_se_,0);
        if (multi_)
        {
            scene_mngr_->ChangeScene(SCENE_MULTI_SERVER_SELECT);
        }
        else
        {
            scene_mngr_->ChangeScene(SCENE_PREP);
        }
    }
    else if(Input::GetKeyDown(0, Input::B))
    {
        scene_mngr_->ChangeScene(SCENE_TITLE);
    }
}

void Menu::Draw()
{
    bg_video_->DrawAsResource();
    float lerp_percent = lerp_timer_.GetNowTime() * 0.001f/ 0.3;
    const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
    const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
    DrawSprite(scene_title_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() -70), 0.0f, // arbirary value of the texture
                   Vector2(650, 650), Color(1.0f, 1.0f, 1.0f, 1.0f));
    DrawSprite(text_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() *4/ 5), 0.0f,
               Vector2(800.0f * scale_x, 200.0f * scale_y), Color(1.0f, 1.0f, 1.0f, 1.0f));
    if (multi_)
    {
        DrawSprite(multi_tex_, Vector2(Graphical::GetWidth() *1/3, Graphical::GetHeight() *2/5), 0.0f,
                   Vector2(450.0f * scale_x, 450.0f * scale_y) * LerpScale(1.0f,1.3,lerp_percent), Color(1.0f, 1.0f, 1.0f, 1.0f));
        DrawSprite(solo_tex_, Vector2(Graphical::GetWidth() *2/3, Graphical::GetHeight() *2/5), 0.0f,
                   Vector2(450.0f * scale_x, 450.0f * scale_y), Color(0.5f, 0.5f, 0.5f, 1.0f));
    }
    else
    {
        DrawSprite(multi_tex_, Vector2(Graphical::GetWidth() *1/3, Graphical::GetHeight() *2/5), 0.0f,
                   Vector2(450.0f * scale_x, 450.0f * scale_y), Color(0.5f, 0.5f, 0.5f, 1.0f));
        DrawSprite(solo_tex_, Vector2(Graphical::GetWidth() *2/3, Graphical::GetHeight() *2/5), 0.0f,
                   Vector2(450.0f * scale_x, 450.0f * scale_y)* LerpScale(1.0f,1.3,lerp_percent), Color(1.0f, 1.0f, 1.0f, 1.0f));
    }
    //return
    DrawSprite(return_tex_, Vector2(((Graphical::GetWidth() -100) * scale_x), 80*scale_y), 0.0f,
               Vector2(80.0f * scale_x, 80.0f * scale_y), Color(1.0f, 1.0f, 1.0f, 1.0f));
    DrawSprite(b_tex_, Vector2(((Graphical::GetWidth() -160) * scale_x), 80*scale_y), 0.0f,
               Vector2(100.0f * scale_x, 100.0f * scale_y), Color(1.0f, 1.0f, 1.0f, 1.0f));
    //confirm
    DrawSprite(confirm_tex_, Vector2(((Graphical::GetWidth() -240) * scale_x), 80*scale_y), 0.0f,
               Vector2(80.0f * scale_x, 80.0f * scale_y), Color(1.0f, 1.0f, 1.0f, 1.0f));
    DrawSprite(a_tex_, Vector2(((Graphical::GetWidth() -305) * scale_x), 80*scale_y), 0.0f,
               Vector2(100.0f * scale_x, 100.0f * scale_y), Color(1.0f, 1.0f, 1.0f, 1.0f));
    //select
    DrawSprite(select_tex_, Vector2(((Graphical::GetWidth() -390) * scale_x), 80*scale_y), 0.0f,
               Vector2(80.0f * scale_x, 80.0f * scale_y), Color(1.0f, 1.0f, 1.0f, 1.0f));
    DrawSprite(stick_tex_, Vector2(((Graphical::GetWidth() -475) * scale_x), 82*scale_y), 0.0f,
               Vector2(115.0f * scale_x, 118.0f * scale_y), Color(1.0f, 1.0f, 1.0f, 1.0f));
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
