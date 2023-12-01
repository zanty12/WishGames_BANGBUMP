#pragma once
#include "gameobject.h"
#include "player.h"

class Camera : public GameObject
{
private:
    Player* player_;
    Vector2 map_size_;
    const Vector2 screen_size_ = Vector2(1920, 1080);

    const float move_border_y_ = 300.0f;//the camera will move with the player if the player touches the border

    const float render_buffer_ = 100.0f; //the camera will render the objects in the buffer zone
public:
    Camera() = delete;
    Camera(Player* player);
    void Update() override;
};
