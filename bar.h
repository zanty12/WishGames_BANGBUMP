#pragma once
#include "animator.h"
#include "lib/vector.h"

class Bar
{
    Vector2 pos_;
    Vector2 size_;
    float percentage_;
    float rot_;
    int tex_;
    Color color_;

public:
    Bar(Vector2 pos, Vector2 size,float rot,Color color) : pos_(pos), size_(size), percentage_(1.0f),rot_(rot), color_(color)
    {
        tex_ = LoadTexture("data/texture/collision.png");
    }
    ~Bar() = default;
    void SetPos(Vector2 pos) { pos_ = pos; }
    void SetPercentage(float percentage) { percentage_ = percentage; }
    void Draw()
    {
        //draw base
        DrawSprite(tex_, pos_, rot_, size_, Color(1.0f, 1.0f, 1.0f, 1.0f));
        //calculate scaling for top
        Vector2 scale = Vector2(size_.x * percentage_, size_.y);
        //move center to left
        Vector2 offset = Vector2(size_.x * (1.0f - percentage_) / 2.0f, 0.0f);
        //calculate offset base on rotation
        offset = Vector2(offset.x * cos(rot_) - offset.y * sin(rot_), offset.x * sin(rot_) + offset.y * cos(rot_));
        offset.x *= -1;
        //draw top
        DrawSprite(tex_, pos_ + offset, rot_, scale, color_);
    }
};
