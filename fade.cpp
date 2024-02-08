#include "fade.h"

#include "graphical.h"
#include "sprite.h"
#include "time.h"

int Fade::fade_tex_ = 0;
float Fade::fade_alpha_ = 0.0f;
bool Fade::fade_out_ = true;
float Fade::fade_rate_ = 1.0f;
bool Fade::finished_ = false;

void Fade::StartFadeIn(float rate)
{
    fade_alpha_ = 0.0f;
    fade_out_ = true;
    fade_rate_ = rate;
    finished_ = false;
    fade_tex_ = LoadTexture("data/texture/UI/fade.png");
}

void Fade::Update()
{
    if(!finished_ && fade_out_)
    {
        fade_alpha_ += Time::GetDeltaTime() * fade_rate_;
        if(fade_alpha_ >= 1.0f)
        {
            fade_out_ = false;
            fade_alpha_ = 1.0f;
        }
        return;
    }

    if(!finished_ && !fade_out_)
    {
        fade_alpha_ -= Time::GetDeltaTime() * fade_rate_;
        if(fade_alpha_ <= 0.0f)
        {
            finished_ = true;
            fade_alpha_ = 0.0f;
        }
    }
}

void Fade::Draw()
{
    Graphical::Clear(Color(0.2f,0.2f,0.2f,1.0f));
    DrawSprite(fade_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 2), 0.0f,
               Vector2(Graphical::GetWidth(), Graphical::GetHeight()),
               Color(0.0f, 0.0f, 0.0f, fade_alpha_));
    Graphical::Present();
}
