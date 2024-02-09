#pragma once
#include "gameobject.h"
#include "movableobj.h"
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
    TitleVer2_State(Title2* title) : title_(title)
    {
    };
    virtual ~TitleVer2_State() = default;
    virtual void Update() = 0;
    virtual void Draw() = 0;
};

class Title2 : public Scene
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
    int overlay_tex_;
    //sound
    int bgm_;
    int confirm_se_;
    int syakin;
    int don;
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
    void Update() override;
    void Draw() override;
    void DebugMenu() override;
    void GoToState(TitleVer2_State* state);

private:
    float AlphaAnimation(float rate = 1.0f);
    float Shake(float input, float range, float rate = 1.0f);

    class Logo : public TitleVer2_State
    {
    public:
        Logo(Title2* title) : TitleVer2_State(title)
        {
        };
        ~Logo() override = default;
        void Update() override;
        void Draw() override;
    };

    class LogoFade : public TitleVer2_State
    {
    public:
        LogoFade(Title2* title) : TitleVer2_State(title)
        {
        };
        ~LogoFade() override = default;
        void Update() override;
        void Draw() override;
    };

    class VideoBase : public TitleVer2_State
    {
    private:
        const float title_scale_start_ = 0.5f;
        float title_scale_ = title_scale_start_;

    public:
        VideoBase(Title2* title) : TitleVer2_State(title)
        {
        };
        ~VideoBase() override = default;
        void Update() override;
        void Draw() override;
    };

    class Flash : public TitleVer2_State
    {
    private:
        float alpha_;

    public:
        Flash(Title2* title) : TitleVer2_State(title)
        {
        };
        ~Flash() override = default;
        void Update() override;
        void Draw() override;
    };
    class AnimatorLite : public MovableObj
    {
    public:
        AnimatorLite(Vector2 pos, float rot, int tex, Vector2 size)
            : MovableObj(pos, rot, tex,Vector2::Zero)
        {
            SetType(OBJ_VOID);
            SetScale(size);
            SetColor(Color(1, 1, 1, 1));
            delete GetCollider();
            SetCollider(nullptr);
            GetAnimator()->SetTexNo(tex);
            GetAnimator()->SetTexenum(effect_EX);
            GetAnimator()->SetLoopAnim(EFFECT_EX_ANIM);
            GetAnimator()->SetIsAnim(true);
            GetAnimator()->SetLoopStart(Vector2::Zero);
            GetAnimator()->SetLoopEnd(Vector2(4, 5));
        }
        ~AnimatorLite() override = default;
        void Update() override
        {
            GetAnimator()->Update();
        }
        void Draw()
        {
            GetAnimator()->Draw(Vector2::Zero);
        }
    };
    class TitleStart : public TitleVer2_State
    {
    private:
        bool first_ = true;
        const float title_scale_start_ = 3.0f;
        float title_scale_ = title_scale_start_;
        float don_timer_ = 0.0f;
        bool don_played_ = false;
        const float base_rot_ =  -10.0f / 180.0f * M_PI;
        float base_rot_angle_ = base_rot_;
        AnimatorLite* revolve_effect_;
        float revolve_timer_ = 1.0f;

    public:
        TitleStart(Title2* title) : TitleVer2_State(title)
        {
            revolve_effect_ = new AnimatorLite(Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() * 2 / 3),0.0f, LoadTexture(Asset::GetAsset(effect_EX)), Vector2(700, 700));
        };
        ~TitleStart() override = default;
        void Update() override;
        void Draw() override;
    };
};
