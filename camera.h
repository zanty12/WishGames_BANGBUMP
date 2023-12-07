#pragma once
#include "player.h"

class Player;
class Camera
{
private:
    Vector2 pos_;
    Vector2 scale_;
    int texNo_;
    Player* player_;
    Vector2 map_size_;
    Vector2 target_pos_;
    const Vector2 screen_size_ = Vector2(1920, 1080);

    const float move_border_y_ = 300.0f;//the camera will move with the player if the player touches the border

    const float render_buffer_ = 100.0f; //the camera will render the objects in the buffer zone
public:
    Camera() = delete;
    Camera(Player* player);
    void Update();
    void Draw()
    {
        const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
        const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
        DrawSprite(texNo_, Vector2((GetPos().x - GetCameraOffset().x) * scale_x, (GetPos().y - GetCameraOffset().y) * scale_y), 0.0f,
                       Vector2(scale_.x * scale_x, scale_.y * scale_y), Color(1.0f,1.0f, 1.0f, 1.0f));
    }
    bool InCamera(GameObject* obj) const;
    Vector2 GetCameraOffset() const
    {
        return Vector2(GetPos().x - screen_size_.x / 2, GetPos().y - screen_size_.y / 2);
    }
    Vector2 GetPos(void) const { return pos_; }
    void SetPos(Vector2 pos) { pos_ = pos; }
    Vector2 GetScale(void) const { return scale_; }
    void SetScale(Vector2 scale) { scale_ = scale; }
    int GetTexNo(void) const { return texNo_; }
    void SetTexNo(int texNo) { texNo_ = texNo; }
};
