#include "camera.h"
#include "asset.h"
#include "gameobject.h"

Camera::Camera(Vector2 start_pos,Vector2 map_size) : pos_(start_pos)
{
    SetTexBack(LoadTexture(Asset::GetAsset(bg_stage1_back)));
    SetTexFront(LoadTexture(Asset::GetAsset(bg_stage1_front)));
    SetScale(screen_size_);
    map_size_.x = map_size.x * GameObject::SIZE_;
    map_size_.y = map_size.y * GameObject::SIZE_;
    SetPos(Vector2(map_size_.x / 2, start_pos.y + screen_size_.y / 2 + GameObject::SIZE_));
}

void Camera::Update(Vector2 pos, Vector2 vel, PLAYER_STATE state)
{
    if (state != TOUCH_GROUND)
    {
        target_pos_ = Vector2(GetPos().x, pos.y);
    }
    else
    {   //player外したからplayer_->GetScale().y / 2はGameObject::SIZE_に変更(2マス分の高さ)
        target_pos_ = Vector2(GetPos().x, pos.y + screen_size_.y / 2 - GameObject::SIZE_ - GameObject::SIZE_);
    }

    if (target_pos_.y < (screen_size_.y / 2))
        target_pos_.y = screen_size_.y / 2;

    //move pos to target pos
    if (GetPos().x != target_pos_.x)
    {
        float new_vel = GetPos().x > target_pos_.x ? -vel.x : vel.x;
        SetPos(Vector2(GetPos().x + new_vel, GetPos().y));
    }
    if (GetPos().y < target_pos_.y - 10.0f || GetPos().y > target_pos_.y + 10.0f)
    {
        float new_vel;
        if (GetPos().y > target_pos_.y)
        {
            if (vel.y != 0.0f)
                new_vel = -abs(vel.y);
            else
                new_vel = -5.0f; //TODO: deltatime調整
        }
        else
        {
            if (vel.y != 0.0f)
                new_vel = abs(vel.y);
            else
                new_vel = 5.0f;
        }

        SetPos(Vector2(GetPos().x, GetPos().y + new_vel));
    }
    //move texture according to pos
    //get the percentage of the camera position in the map
    float y_percent = GetPos().y / map_size_.y;
    //move texture up and down according to the percentage
    bg_back_y_ = 1.0f - (540.0f * y_percent) / 1620.0f; //back texture is 1920 * 1620 scaled up
    bg_front_y_ = 1.0f - (4320.0f * y_percent) / 5400.0f; //front texture is 1920 * 5400 scaled up
}

bool Camera::InCamera(Vector2 pos) const
{
    if (GetPos().x > pos.x - screen_size_.x / 2 - render_buffer_ && GetPos().x < pos.x +
        screen_size_.x / 2 + render_buffer_)
    {
        if (GetPos().y > pos.y - screen_size_.y / 2 - render_buffer_ && GetPos().y < pos.y +
            screen_size_.y / 2 + render_buffer_)
        {
            return true;
        }
    }
    return false;
}
