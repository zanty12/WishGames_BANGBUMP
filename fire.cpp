#include "fire.h"

#include <libavutil/mathematics.h>

#include "asset.h"
#include "xinput.h"
#include "sprite.h"
#include"lib/collider2d.h"

Vector2 Fire::Move()
{
    Vector2 stick = Input::GetStickLeft(0);
    stick.y *= -1;
    if (stick.Distance() > responseMinStickDistance && player_->GetVel().Distance() < speed * Time::GetDeltaTime())
    {
        Vector2 dir = stick.Normalize();
        Vector2 vel = player_->GetVel() + dir * speed * Time::GetDeltaTime() * Time::GetDeltaTime();
        Vector2 player_dir = player_->GetVel().Normalize();

        float angle = acos(Vector2::Dot(dir, player_dir));
        if (acos(Vector2::Dot(dir, player_dir)) > M_PI_2)
            vel += dir * speed * Time::GetDeltaTime() * Time::GetDeltaTime();
        player_->SetGravityState(GRAVITY_NONE);
        return vel;
    }
    else if (stick.Distance() > responseMinStickDistance && player_->GetVel().Distance() > speed * Time::GetDeltaTime())
    {
        Vector2 dir = stick.Normalize();
        Vector2 vel = dir * speed * Time::GetDeltaTime();
        player_->SetGravityState(GRAVITY_NONE);
        return vel;
    }
    else
    {
        player_->SetGravityState(GRAVITY_FULL);
        if (player_->GetVel().Distance() > Player::GRAVITY_SCALE_ * Time::GetDeltaTime())
            return player_->GetVel() * friction;
        else
            return player_->GetVel();
    }
};

void Fire::Action()
{
    using namespace PHYSICS;
    Vector2 stick = Input::GetStickRight(0);

    if (responseMinStickDistance < stick.Distance())
    {
        //get angle from stick
        float angle = atan2(stick.y, stick.x);
        if (attack_ == nullptr)
            attack_ = new FireAttack(this);
        //get pos of attack from angle
        Vector2 pos = Vector2(cos(angle), -sin(angle)) * (player_->GetScale().x / 2 + attack_->GetScale().x / 2);
        pos = player_->GetPos() + pos;
        attack_->SetPos(pos);
        attack_->SetRot(angle);
    }
    else
    {
        if (attack_ != nullptr)
        {
            delete attack_;
            attack_ = nullptr;
        }
    }
}

void Fire::DebugMenu()
{
    ImGui::Begin("Fire");
    ImGui::SliderFloat2("speed", &speed, 0.0f, 9 * GameObject::SIZE_);
    ImGui::End();
}

FireAttack::FireAttack(Fire* parent) : parent_(parent),
                                       MovableObj(
                                           Vector2(
                                               parent->GetPlayer()->GetPos().x + parent->GetPlayer()->GetScale().x / 2 +
                                               1.5 * GameObject::SIZE_, parent->GetPlayer()->GetPos().y), 0.0f,
                                           LoadTexture(Asset::GetAsset(fire_attack)), Vector2::Zero),PlayerAttack(10000)
{
    SetScale(size_);
    SetType(OBJ_ATTACK);
}
