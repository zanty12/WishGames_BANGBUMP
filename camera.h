#pragma once
#include "playerstate.h"
#include "graphical.h"
#include "sprite.h"

class Player;
class Camera
{
private:
    Vector2 pos_;
    Vector2 scale_;
    Vector2 map_size_;
    Vector2 target_pos_;
    const Vector2 screen_size_ = Vector2(1920, 1080);

    const float move_border_y_ = 300.0f; //the camera will move with the player if the player touches the border

    const float render_buffer_ = 100.0f; //the camera will render the objects in the buffer zone

    int bg_back_tex_;
    float bg_back_y_ = 1.0f;
    int bg_front_tex_;
    float bg_front_y_ = 1.0f;

public:
    Camera() = delete;
    Camera(Vector2 start_pos,Vector2 map_size);
    void Update(Vector2 pos,Vector2 vel,PLAYER_STATE state);

    void Draw()
    {
        const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
        const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
        Vector2 draw_vertices[4] = {
            Vector2(0.0f, 0.0f),
            Vector2(0.0f, 1080.0f * scale_y),
            Vector2(1920.0f * scale_x, 0.0f),
            Vector2(1920.0f *scale_x, 1080.0f * scale_y),
        };
        Vector2 draw_uv_back[4] = {
            Vector2(0.0f, bg_back_y_ - 1080.0f / 1620.0f),
            Vector2(0.0f, bg_back_y_),
            Vector2(1.0f, bg_back_y_ - 1080.0f / 1620.0f),
            Vector2(1.0f, bg_back_y_),
        };
        Vector2 draw_uv_front[4] = {
            Vector2(0.0f, bg_front_y_ - 1080.0f / 5400.0f),
            Vector2(0.0f, bg_front_y_),
            Vector2(1.0f, bg_front_y_ - 1080.0f / 5400.0f),
            Vector2(1.0f, bg_front_y_),
        };
        DrawSprite(bg_back_tex_, draw_vertices, draw_uv_back, Color(1.0f, 1.0f, 1.0f, 1.0f));
        DrawSprite(bg_front_tex_, draw_vertices, draw_uv_front, Color(1.0f, 1.0f, 1.0f, 1.0f));
    }

    bool InCamera(Vector2 pos) const;

    Vector2 GetCameraOffset() const
    {
        return Vector2(GetPos().x - screen_size_.x / 2, GetPos().y - screen_size_.y / 2);
    }

    Vector2 GetPos(void) const { return pos_; }
    void SetPos(Vector2 pos) { pos_ = pos; }
    Vector2 GetScale(void) const { return scale_; }
    void SetScale(Vector2 scale) { scale_ = scale; }
    int GetTexBack(void) const { return bg_back_tex_; }
    void SetTexBack(int texNo) { bg_back_tex_ = texNo; }
    int GetTexFront(void) const { return bg_front_tex_; }
    void SetTexFront(int texNo) { bg_front_tex_ = texNo; }
};
