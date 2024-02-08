#pragma once
#include "texture.h"
#include "lib/win_time.h"

class Fade
{
private:
    static int fade_tex_;
    static float fade_alpha_;
    static bool fade_out_;
    static float fade_rate_;
    static bool finished_;
public:
    Fade() = default;
    ~Fade() = default;
    static void StartFadeIn(float rate);
    static void Update();
    static void Draw();
    static bool Finished()
    {
        return finished_;
    }
};
