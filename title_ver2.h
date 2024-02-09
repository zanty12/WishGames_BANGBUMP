#pragma once
#include "sprite.h"
#include "scene.h"
#include "scenemngr.h"
#include "video.h"

class Title2;

class TitleVer2_State
{
protected:
    Title2* title_;
public:
    TitleVer2_State(Title2* title) : title_(title) {};
    virtual ~TitleVer2_State() = default;
    virtual void Update() = 0;
    virtual void Draw() = 0;
};

class Title2 :public Scene
{
protected:
    SceneMngr* scene_mngr_;
    TitleVer2_State* state_;
    //graphics
    Video* title_video_;
    Video* title_video_base_;
    int logo_tex_;
    int press_button_tex_;
    int title_tex_;
    int title_base_tex_;
    int flash_tex_;
    //sound
    int bgm_;
    int confirm_se_;
    //control
    //logo
    bool logo_ = true;
    float logo_time_ = 3.0f;
    float logo_alpha_ = 0.0f;
    //fade
    float fade_alpha_ = 1.0f;
    //video base
    const float video_base_time_ = 5.78f; //prelude of bgm
    float video_base_timer_ = video_base_time_;
    //flash
    const float video_flash_time_ = 0.23f; //arbitrary flash time
    float video_flash_timer_ = video_flash_time_;
    //control
    bool game_start_ = false;
    float game_start_wait_ = 1.0f;


public:
    Title2(SceneMngr* scene_mngr);
    ~Title2();
    void Update()override;
    void Draw()override;
    void DebugMenu() override;
    void GoToState(TitleVer2_State* state);

private:
    float AlphaAnimation(float rate = 1.0f);

    class Logo : public TitleVer2_State
    {
    public:
        Logo(Title2* title) : TitleVer2_State(title) {};
        ~Logo() override = default;
        void Update() override;
        void Draw() override;
    };

    class LogoFade : public TitleVer2_State
    {
    public:
        LogoFade(Title2* title) : TitleVer2_State(title) {};
        ~LogoFade() override = default;
        void Update() override;
        void Draw() override;
    };

    class VideoBase : public TitleVer2_State
    {
    public:
        VideoBase(Title2* title) : TitleVer2_State(title) {};
        ~VideoBase() override = default;
        void Update() override;
        void Draw() override;
    };

    class Flash : public TitleVer2_State
    {
    private:
        float alpha_;
    public:
        Flash(Title2* title) : TitleVer2_State(title) {};
        ~Flash() override = default;
        void Update() override;
        void Draw() override;
    };

    class TitleStart : public TitleVer2_State
    {
    public:
        TitleStart(Title2* title) : TitleVer2_State(title) {};
        ~TitleStart() override = default;
        void Update() override;
        void Draw() override;
    };

};
