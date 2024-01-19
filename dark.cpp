//--------------------------------------------------------------------------------
// 
// ワープ[fark.cpp]
// 
// 作成者 趙潤博
// 
// 最終更新日	2023/11/22
// 
//--------------------------------------------------------------------------------
#include"dark.h"

#include "asset.h"
#include"xinput.h"
#include"lib/collider2d.h"


Vector2 Dark::Move()
{
    Vector2 velocity = player_->GetVel();

    if (Input::GetKey(0, Input::LThumb))
    {
        if (velocity.y <= -maxSpeedFalling)
        {
            velocity.y = maxSpeedFalling;
            return velocity;
        }
    }
    if (Input::GetKeyUp(0, Input::LThumb))
    {
        Vector2 stick = Input::GetStickLeft(0);
        stick.y *= -1;
        Vector2 warpDirection = stick * warpDistance;
        warpPosition = player_->GetPos() + warpDirection;

        player_->SetPos(warpPosition);
    }
    else warpPosition = Vector2::Zero;


    return player_->GetVel();
};

void Dark::Action()
{
    using namespace PHYSICS;
    Vector2 stick = Input::GetStickRight(0);
    //float distance = stick.Distance();

    // 押し込む
    if (Input::GetKeyDown(0, Input::RThumb) && responseMinStickDistance < stick.Distance())
    {
        attackDirection = stick.Normalize();
    }
    // 攻撃
    else if (Input::GetKey(0, Input::RThumb))
    {
        float angle = atan2(attackDirection.y, attackDirection.x);
        if (attack_ == nullptr)
            attack_ = new DarkAttack(this);
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

void Dark::DebugMenu()
{
    ImGui::Begin("Dark");
    ImGui::SliderFloat2("maxSpeedFalling", &maxSpeedFalling, 0.0f, 1.0f);
    ImGui::SliderFloat2("warpDistance", &warpDistance, 400.0f, 1000.0f);
    ImGui::End();
}

DarkAttack::DarkAttack(Dark* parent) : parent_(parent),
                                       MovableObj(
                                           Vector2(
                                               parent->GetPlayer()->GetPos().x + parent->GetPlayer()->GetScale().x / 2 +
                                               5 * GameObject::SIZE_, parent->GetPlayer()->GetPos().y), 0.0f,
                                           LoadTexture(Asset::GetAsset(water_attack)), Vector2::Zero)
{
    SetScale(size_);
    SetType(OBJ_ATTACK);
}
