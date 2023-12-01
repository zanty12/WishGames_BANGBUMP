#include "camera.h"

Camera::Camera (Player* player) : GameObject(player->GetPos(),0.0f,0),player_(player)
{
    SetTexNo(LoadTexture("data/texture/background.png"));
    SetScale(screen_size_);
    map_size_.x = player->GetMapMngr()->GetMap()->GetWidth() * GameObject::size_;
    map_size_.y = player->GetMapMngr()->GetMap()->GetHeight() * GameObject::size_;
    SetPos(Vector2(map_size_.x / 2, player->GetPos().y ));
};

void Camera::Update()
{

}