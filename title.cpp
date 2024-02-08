#include "title.h"
#include "xinput.h"
#include "asset.h"
#include "sound.h"

Title::Title(SceneMngr* scene_mngr)
    : scene_mngr_(scene_mngr)
{
    title_video_ = new Video("data/video/title.mp4");
    logo_tex_ = LoadTexture("data/texture/UI/title/team_logo.png");
    press_button_tex_ = LoadTexture("data/texture/UI/title/pressanybutton.png");
    title_tex_ = LoadTexture("data/texture/UI/title/team_logo.png");
    title_video_->SetLoop(true);
    title_video_->SetWindowPos(Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 2));
    title_video_->SetSize(Vector2(Graphical::GetWidth(), Graphical::GetHeight()));
    bgm_ = LoadSound("data/sound/bgm/title_BGM.wav");
    confirm_se_ = LoadSound("data/sound/se/gamestart.wav");
    SetVolume(bgm_, 0.4f);
}

Title::~Title()
{
    StopSoundAll();
}

void Title::Update()
{
    //if logo
    if (logo_)
    {
        logo_time_ -= Time::GetDeltaTime();
        if (logo_alpha_ < 1.0f)
            logo_alpha_ += Time::GetDeltaTime() / 2.0f;
        if (logo_alpha_ > 1.0f)
            logo_alpha_ = 1.0f;
        if (logo_time_ <= 0.0f)
        {
            logo_ = false;
        }
    }
    else if (!logo_ && fade_alpha_ <= 0.0f)
    {
        title_video_->Update();
    }
    if (!logo_ && video_fade_ >= 1.0f && !bgm_start_)
    {
        bgm_start_ = true;
        PlaySound(bgm_, -1);
    }


    if ((Input::GetKeyDown(0, Input::A) || Input::GetKeyDown(0, Input::B) || Input::GetKeyDown(0, Input::X) ||
        Input::GetKeyDown(0, Input::Y) || Input::GetKeyDown(0, Input::L) || Input::GetKeyDown(0, Input::R) ||
        Input::GetKeyDown(0, Input::Up) || Input::GetKeyDown(0, Input::Down) || Input::GetKeyDown(0, Input::Left) ||
        Input::GetKeyDown(0, Input::Right)) && !game_start_) //Aボタン
    {
        if (logo_)
        {
            logo_ = false;
        }
        else
        {
            PlaySound(confirm_se_, 0);
            game_start_ = true;
        }
    }
    if (game_start_ && game_start_wait_ > 0.0f)
    {
        game_start_wait_ -= Time::GetDeltaTime();
    }
    if (game_start_ && game_start_wait_ <= 0.0f)
    {
        scene_mngr_->ChangeScene(SCENE_MENU);
    }
}

void Title::Draw()
{
    const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
    const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
    if (logo_)
    {
        Graphical::Clear(Color::White);
        DrawSprite(logo_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 2), 0.0f,
                   Vector2(1088 * scale_x, 177 * scale_y), Color(1.0f, 1.0f, 1.0f, logo_alpha_));
    }
    else
    {
        //draw fade in
        if (fade_alpha_ > 0.0f)
        {
            //because alpha is 0.0f to 1.0f, we just use alpha as time
            fade_alpha_ -= Time::GetDeltaTime() / 0.3f;
            Graphical::Clear(Color(fade_alpha_, fade_alpha_, fade_alpha_, 1.0f));
            return;
        }
        if (video_fade_ < 1.0f)
            video_fade_ += Time::GetDeltaTime() / 0.5f;
        title_video_->DrawAsResource(Color(1.0f, 1.0f, 1.0f, video_fade_));
        if (title_scale_ > 1.0f)
            title_scale_ -= (title_scale_start_ / 0.5f) * Time::GetDeltaTime();
        if (title_scale_ < 1.0f)
            title_scale_ = 1.0f;

        DrawSprite(title_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() * 2 / 3), 0.0f,
                   Vector2(1088 * scale_x, 177 * scale_y) * title_scale_, Color(1.0f, 1.0f, 1.0f, logo_alpha_));
        if (title_scale_ <= 1.0f)
        {
            if (game_start_)
            {
                DrawSprite(press_button_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 3), 0.0f,
                           Vector2(999 * scale_x, 100.3 * scale_y), Color(1.0f, 1.0f, 1.0f, AlphaAnimation(20.0f)));
            }
            DrawSprite(press_button_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 3), 0.0f,
                       Vector2(999 * scale_x, 100.3 * scale_y), Color(1.0f, 1.0f, 1.0f, AlphaAnimation()));
        }
    }
}

void Title::DebugMenu()
{
    ImGui::Begin(u8"タイトル");
    if (ImGui::Button(u8"ゲームスタート"))
    {
        scene_mngr_->ChangeScene(SCENE_MENU);
    }
    ImGui::End();
}

float Title::AlphaAnimation(float rate)
{
    //sin波でアルファ値を変化させる
    //rateで点滅の速さを変える
    static float time;
    time += Time::GetDeltaTime() * rate;
    return (sin(time) + 1.0f) / 2.0f;
}
