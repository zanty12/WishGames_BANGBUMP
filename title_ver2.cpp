#include "title_ver2.h"
#include "xinput.h"
#include "asset.h"
#include "sound.h"

Title2::Title2(SceneMngr* scene_mngr)
    : scene_mngr_(scene_mngr)
{
    //texture
    logo_tex_ = LoadTexture("data/texture/UI/title/team_logo.png");
    press_button_tex_ = LoadTexture("data/texture/UI/title/pressanybutton.png");
    title_base_tex_ = LoadTexture("data/texture/UI/title/title_base.png");
    title_tex_ = LoadTexture("data/texture/UI/title/title.png");
    flash_tex_ = LoadTexture("data/texture/UI/fade_white.png");
    overlay_tex_ = LoadTexture("data/texture/UI/fade.png");
    //video
    title_video_ = new Video("data/video/title.mp4");
    title_video_->SetLoop(true);
    title_video_->SetWindowPos(Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 2));
    title_video_->SetSize(Vector2(Graphical::GetWidth(), Graphical::GetHeight()));
    title_video_base_ = new Video("data/video/movie_title_BG.mp4");
    title_video_base_->SetLoop(true);
    title_video_base_->SetWindowPos(Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 2));
    title_video_base_->SetSize(Vector2(Graphical::GetWidth(), Graphical::GetHeight()));
    //sound
    bgm_ = LoadSound("data/sound/bgm/title_BGM_long.wav");
    confirm_se_ = LoadSound("data/sound/se/gamestart.wav");
    syakin = LoadSound("data/sound/se/syakin.wav");
    don = LoadSound("data/sound/se/don.wav");
    SetVolume(bgm_, 0.4f);

    //state
    state_ = new Logo(this);
}

Title2::~Title2()
{
    delete title_video_;
    delete title_video_base_;
    StopSoundAll();
}

void Title2::Update()
{
    state_->Update();
}

void Title2::Draw()
{
    state_->Draw();
}

void Title2::DebugMenu()
{
    ImGui::Begin(u8"タイトル");
    if (ImGui::Button(u8"ゲームスタート"))
    {
        scene_mngr_->ChangeScene(SCENE_MENU);
    }
    ImGui::End();
}

float Title2::AlphaAnimation(float rate)
{
    static float time;
    time += Time::GetDeltaTime() * rate;
    return (sinf(time) + 1.0f) / 2.0f;
}

float Title2::Shake(float input, float range, float rate)
{
    static float time;
    time += Time::GetDeltaTime() * rate;
    return input + sinf(time) * range;
}

void Title2::GoToState(TitleVer2_State* state)
{
    delete state_;
    state_ = state;
}

void Title2::Logo::Update()
{
    title_->logo_time_ -= Time::GetDeltaTime();
    if (title_->logo_alpha_ < 1.0f)
        title_->logo_alpha_ += Time::GetDeltaTime() / 2.0f;
    if (title_->logo_alpha_ > 1.0f)
        title_->logo_alpha_ = 1.0f;
    if (title_->logo_time_ <= 0.0f)
    {
        PlaySound(title_->bgm_, -1);
        title_->GoToState(new LogoFade(title_));
    }
    if (Input::GetKeyDown(0, Input::A) || Input::GetKeyDown(0, Input::B) || Input::GetKeyDown(0, Input::X) ||
        Input::GetKeyDown(0, Input::Y) || Input::GetKeyDown(0, Input::L) || Input::GetKeyDown(0, Input::R) ||
        Input::GetKeyDown(0, Input::Up) || Input::GetKeyDown(0, Input::Down) || Input::GetKeyDown(0, Input::Left) ||
        Input::GetKeyDown(0, Input::Right)) //Aボタン
    {
        title_->logo_time_ = 0.0f;
    }
}

void Title2::Logo::Draw()
{
    const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
    const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
    Graphical::Clear(Color::White);
    DrawSprite(title_->logo_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 2), 0.0f,
               Vector2(1088 * scale_x, 177 * scale_y), Color(1.0f, 1.0f, 1.0f, title_->logo_alpha_));
}

void Title2::LogoFade::Update()
{
    title_->fade_alpha_ -= Time::GetDeltaTime() / 0.3f;
    if (title_->fade_alpha_ <= 0.0f)
    {
        title_->GoToState(new VideoBase(title_));
    }
}

void Title2::LogoFade::Draw()
{
    Graphical::Clear(Color(title_->fade_alpha_, title_->fade_alpha_, title_->fade_alpha_, 1.0f));
}

void Title2::VideoBase::Update()
{
    title_->video_base_timer_ -= Time::GetDeltaTime();
    title_->title_video_base_->Update();
    title_scale_ = title_scale_start_ + (1.0f - title_scale_start_) * (title_->video_base_time_ - title_->
        video_base_timer_) / title_->video_base_time_;

    if (title_->video_base_timer_ <= title_->video_flash_time_)
    {
        title_->GoToState(new Flash(title_));
    }
}

void Title2::VideoBase::Draw()
{
    const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
    const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
    title_->title_video_base_->DrawAsResource(Color(1.0f, 1.0f, 1.0f, 1.0f));
    DrawSprite(title_->title_base_tex_, Vector2(Graphical::GetWidth() / 2,
                                                Graphical::GetHeight() * 2 / 3), 0.0f,
               Vector2(960, 960) * title_scale_, Color(0.0f, 0.0f, 0.0f, 1.0f));
    DrawSprite(title_->overlay_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 2), 0.0f,
               Vector2(1920 * scale_x, 1080 * scale_y), Color(1.0f, 1.0f, 1.0f, 0.8));
}

void Title2::Flash::Update()
{
    title_->video_flash_timer_ -= Time::GetDeltaTime();
    title_->title_video_base_->Update();
    //calculate alpha by time
    alpha_ = 1.0f - (title_->video_flash_time_ - title_->video_flash_timer_) / title_->video_flash_time_;
    if (alpha_ < 0.7f)
    {
        delete title_->title_video_base_;
        title_->title_video_base_ = nullptr;
        title_->GoToState(new TitleStart(title_));
    }
}

void Title2::Flash::Draw()
{
    const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
    const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
    title_->title_video_base_->DrawAsResource(Color(1.0f, 1.0f, 1.0f, 1.0f));
    DrawSprite(title_->title_base_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() * 2 / 3), 0.0f,
               Vector2(960 * scale_x, 960 * scale_y), Color(1.0f, 1.0f, 1.0f, title_->logo_alpha_));
    DrawSprite(title_->flash_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 2), 0.0f,
               Vector2(1920 * scale_x, 1080 * scale_y), Color(1.0f, 1.0f, 1.0f, alpha_));
}

void Title2::TitleStart::Update()
{
    title_->title_video_->Update();
    if (title_scale_ > 1.0f)
        title_scale_ -= (title_scale_start_) * Time::GetDeltaTime();
    if (first_)
    {
        first_ = false;
        PlaySound(title_->syakin, 0);
    }
    if (title_scale_ < 1.0f)
    {
        title_scale_ = 1.0f;
    }
    if (don_timer_ < 1.1f)
        don_timer_ += Time::GetDeltaTime();
    if (don_timer_ >= 1.1f && !don_played_)
    {
        don_played_ = true;
        PlaySound(title_->don, 0);
    }
    if (don_timer_ > 1.0f && !don_played_ && base_rot_angle_ < 0.0f)
    {
        base_rot_angle_ += abs(base_rot_) * Time::GetDeltaTime() * 10.0f;
    }
    if(don_played_ && revolve_timer_ > 0.0f)
    {
        revolve_timer_ -= Time::GetDeltaTime();
        revolve_effect_->Update();
    }
    if ((Input::GetKeyDown(0, Input::A) || Input::GetKeyDown(0, Input::B) || Input::GetKeyDown(0, Input::X) ||
        Input::GetKeyDown(0, Input::Y) || Input::GetKeyDown(0, Input::L) || Input::GetKeyDown(0, Input::R) ||
        Input::GetKeyDown(0, Input::Up) || Input::GetKeyDown(0, Input::Down) || Input::GetKeyDown(0, Input::Left) ||
        Input::GetKeyDown(0, Input::Right)) && !title_->game_start_) //Aボタン
    {
        PlaySound(title_->confirm_se_, 0);
        title_->game_start_ = true;
    }
    if (title_->game_start_ && title_->game_start_wait_ > 0.0f)
    {
        title_->game_start_wait_ -= Time::GetDeltaTime();
    }
    if (title_->game_start_ && title_->game_start_wait_ <= 0.0f)
    {
        title_->scene_mngr_->ChangeScene(SCENE_MENU);
    }
}

void Title2::TitleStart::Draw()
{
    title_->title_video_->DrawAsResource(Color(1.0f, 1.0f, 1.0f, 1.0f));
    DrawSprite(title_->title_base_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() * 2 / 3), base_rot_angle_,
               Vector2(960, 960), Color(1.0f, 1.0f, 1.0f, 1.0f));
    DrawSprite(title_->title_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() * 2 / 3), 0.0f,
               Vector2(960, 960) * title_scale_, Color(1.0f, 1.0f, 1.0f, 1.0f));
    if(don_played_ && revolve_timer_ > 0.0f)
    {
        revolve_effect_->Draw();
    }
    if (!title_->game_start_)
        DrawSprite(title_->press_button_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 3), 0.0f,
                   Vector2(999, 100.3), Color(1.0f, 1.0f, 1.0f, title_->AlphaAnimation()));
    else
    {
        DrawSprite(title_->press_button_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 3), 0.0f,
                   Vector2(999, 100.3), Color(1.0f, 1.0f, 1.0f, title_->AlphaAnimation(20.0f)));
    }
}
