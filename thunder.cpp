//--------------------------------------------------------------------------------
// 
// サンダー[thunder.cpp]
// 
// 作成者 趙潤博
// 
// 最終更新日	2023/11/22
// 
//--------------------------------------------------------------------------------
#include "thunder.h"

#include "asset.h"
#include"xinput.h"
#include"lib/collider2d.h"

Thunder::~Thunder()
{
    for (auto& a : attack_)
    {
        if (a != nullptr)
        {
            delete a;
        }
    }
}


bool Thunder::StickTrigger(Vector2 stick, Vector2 previousStick)
{
    stick.y *= -1;
    previousStick.y *= -1;
    float distance = stick.Distance();
    float previousDistance = previousStick.Distance();

    //Charge
    if (responseMinStickDistance <= distance)
    {
        charge_ += distance;
        player_->SetGravityState(GRAVITY_NONE);
        if (maxCharge_ < charge_) charge_ = maxCharge_;
    }

    //Release
    if (distance < responseMinStickDistance && responseMinStickDistance < previousDistance &&
        responseMinOneFrameDistance < (stick - previousStick).Distance())
    {
        return true;
    }
    return false;
}

Vector2 Thunder::Move()
{
    Vector2 stick = Input::GetStickLeft(0);
    Vector2 previousStick = Input::GetPreviousStickLeft(0);
    stick.y *= -1;
    previousStick.y *= -1;
    float distance = stick.Distance();

    if (StickTrigger(stick, previousStick))
    {
        move_dir_ = -previousStick.Normalize();
        move_cd_ = move_cd_max_;
        moving_ = true;
    }

    if (moving_)
    {
        Vector2 direction = move_dir_ * movePower * Time::GetDeltaTime();
        player_->SetGravityState(GRAVITY_NONE);
        charge_ -= maxCharge_ * Time::GetDeltaTime();
        if (charge_ < 0.0f)
        {
            charge_ = 0.0f;
            moving_ = false;
            player_->SetGravityState(GRAVITY_FULL);
        }
        return direction;
    }

    if (stick != Vector2::Zero)
    {
        player_->SetGravityState(GRAVITY_NONE);
    }
    return player_->GetVel();
}

void Thunder::Action()
{
    //cd
    if (attack_cd_ > 0.0f)
        attack_cd_ -= Time::GetDeltaTime();
    if (attack_cd_ < 0.0f)
        attack_cd_ = 0.0f;
    //update attack
    for (int i = 0; i < 5; i++)
    {
        if (attack_[i] != nullptr)
        {
            attack_[i]->Update();
            if (attack_[i]->GetDiscard())
            {
                delete attack_[i];
                attack_[i] = nullptr;
            }
        }
    }
    using namespace PHYSICS;
    Vector2 stick = Input::GetStickRight(0);
    Vector2 previousStick = Input::GetPreviousStickRight(0);
    stick.y *= -1;
    previousStick.y *= -1;
    float stick_distance = stick.Distance();

    //charge up
    if (stick_distance >= responseMinStickDistance)
    {
        attack_charge_ += Time::GetDeltaTime();
        if (attack_charge_ >= attack_charge_max_)
        {
            attack_charge_ = attack_charge_max_;
        }
    }
    //release
    if (attack_charge_ > atttack_trigger_min_ && stick_distance < responseMinStickDistance && attack_cd_ <= 0.0f)
    {
        for (int i = 0; i < 5; i++)
        {
            if (attack_[i] == nullptr)
            {
                float range = 1 + (attack_charge_ - atttack_trigger_min_) / (attack_charge_max_- atttack_trigger_min_)*(15 + 1) * GameObject::SIZE_;
                //15の後はレベル変動値
                attack_[i] = new ThunderAttack(this, -previousStick.Normalize(),
                                               17 * GameObject::SIZE_ * Time::GetDeltaTime(),range);
                attack_charge_ = 0.0f;
                attack_cd_ = 1.0f;
                break;
            }
        }
    }
}


void Thunder::DebugMenu()
{
    ImGui::Begin("Thunder");
    ImGui::Text("charge:%f", charge_);
    ImGui::Text("attack_charge:%f", attack_charge_);
    ImGui::SliderFloat("maxCharge", &maxCharge_, 0.0f, 100.0f);
    ImGui::SliderFloat("movePower", &movePower, 0.0f, 5.0f);
    ImGui::End();
}

ThunderAttack::ThunderAttack(Thunder* parent, Vector2 dir, float vel,float range): parent_(parent),range_(range),
                                                                       MovableObj(parent->GetPlayer()->GetPos(),
                                                                           atan2(-dir.y, dir.x),
                                                                           LoadTexture(Asset::GetAsset(fire_attack)),
                                                                           dir * vel)
{
    start_pos_ = parent_->GetPlayer()->GetPos();
    SetPos(parent_->GetPlayer()->GetPos());
    SetScale(size_);
    SetType(OBJ_VOID);
}

void ThunderAttack::Update()
{
    if ((GetPos() - start_pos_).Distance() > range_)
        Discard();
    AddVel(GetVel());
}
