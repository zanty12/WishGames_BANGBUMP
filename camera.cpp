#include "camera.h"

Camera::Camera(Player* player) : pos_(player->GetPos()), player_(player)
{
    SetTexNo(LoadTexture("data/texture/bg.png"));
    SetScale(screen_size_);
    map_size_.x = player->GetMapMngr()->GetMap()->GetWidth() * GameObject::SIZE_;
    map_size_.y = player->GetMapMngr()->GetMap()->GetHeight() * GameObject::SIZE_;
    SetPos(Vector2(map_size_.x / 2, player->GetPos().y + screen_size_.y / 2 + GameObject::SIZE_));
};

void Camera::Update()
{
    if (player_->GetPlayerState() != TOUCH_GROUND)
    {
        target_pos_ = Vector2(GetPos().x, player_->GetPos().y);
    }
    else
    {
        target_pos_ = Vector2(GetPos().x, player_->GetPos().y + screen_size_.y / 2 - player_->GetScale().y / 2 - GameObject::SIZE_);
    }

    if(target_pos_.y < (screen_size_.y / 2))
        target_pos_.y = screen_size_.y / 2;

    //move pos to target pos
    if(GetPos().x != target_pos_.x)
    {
        float vel = GetPos().x > target_pos_.x ? -player_->GetVel().x : player_->GetVel().x;
        SetPos(Vector2(GetPos().x + vel, GetPos().y));
    }
    if (GetPos().y < target_pos_.y - 10.0f || GetPos().y > target_pos_.y + 10.0f)
    {
        float vel;
        if(GetPos().y > target_pos_.y)
        {
            if(player_->GetVel().y != 0.0f)
                vel = -abs(player_->GetVel().y);
            else
                vel = -5.0f;
        }
        else
        {
            if(player_->GetVel().y != 0.0f)
                vel = abs(player_->GetVel().y);
            else
                vel = 5.0f;
        }

        SetPos(Vector2(GetPos().x, GetPos().y + vel));
    }
}

bool Camera::InCamera(GameObject* obj) const
{
    if (GetPos().x > obj->GetPos().x - screen_size_.x / 2 - render_buffer_ && GetPos().x < obj->GetPos().x +
        screen_size_.x / 2 + render_buffer_)
    {
        if (GetPos().y > obj->GetPos().y - screen_size_.y / 2 - render_buffer_ && GetPos().y < obj->GetPos().y +
            screen_size_.y / 2 + render_buffer_)
        {
            return true;
        }
    }
    return false;
}
