#include "enemy.h"
#include "camera.h"

void Enemy::Draw(Camera* camera)
{
    GameObject::Draw(camera->GetCameraOffset());
}
