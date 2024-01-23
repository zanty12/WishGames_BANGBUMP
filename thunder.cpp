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
        move_charge_ += distance;
        player_->SetGravityState(GRAVITY_NONE);
        if (move_charge_max_ < move_charge_) move_charge_ = move_charge_max_;
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
    using namespace PHYSICS;
    Vector2 stick = Input::GetStickLeft(0);
    Vector2 previousStick = Input::GetPreviousStickLeft(0);
    stick.y *= -1;
    previousStick.y *= -1;
    float stick_distance = stick.Distance();

    //charge up
    if (stick_distance >= responseMinStickDistance)
    {
        move_charge_ += Time::GetDeltaTime();
        if (move_charge_ >= move_charge_max_)
        {
            move_charge_ = move_charge_max_;
        }
        player_->SetGravityState(GRAVITY_NONE);
        //put indicator
        if(move_indicator_ == nullptr)
        {
            move_indicator_ = new ThunderIndicator();
        }
        float angle = atan2(previousStick.y, -previousStick.x);
        Vector2 pos = Vector2(cos(angle), -sin(angle)) * (player_->GetScale().x / 2 + move_indicator_->GetScale().x / 2);
        pos = player_->GetPos() + pos;
        move_indicator_->SetPos(pos);
        move_indicator_->SetRot(angle);
    }
    //release
    if (move_charge_ > move_trigger_min_ && stick_distance < responseMinStickDistance && move_cd_ <= 0.0f)
    {
        move_dir_ = -previousStick.Normalize();
        move_charge_ = 0.0f;
        move_cd_ = 1.0f;
        delete move_indicator_;
        move_indicator_ = nullptr;
        moving_ = true;
    }
    if (moving_)
    {
        Vector2 direction = move_dir_ * movePower * Time::GetDeltaTime();
        player_->SetGravityState(GRAVITY_NONE);
        move_charge_ -= move_charge_max_ * Time::GetDeltaTime();
        if (move_charge_ < 0.0f)
        {
            move_charge_ = 0.0f;
            moving_ = false;
            player_->SetGravityState(GRAVITY_FULL);
        }
        return direction;
    }
    else
    {
        move_cd_ -= Time::GetDeltaTime();
        if (move_cd_ < 0.0f)
            move_cd_ = 0.0f;
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
        //put indicator
        if(attack_indicator_ == nullptr)
        {
            attack_indicator_ = new ThunderIndicator();
        }
        float angle = atan2(previousStick.y, -previousStick.x);
        Vector2 pos = Vector2(cos(angle), -sin(angle)) * (player_->GetScale().x / 2 + attack_indicator_->GetScale().x / 2);
        pos = player_->GetPos() + pos;
        attack_indicator_->SetPos(pos);
        attack_indicator_->SetRot(angle);
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
        delete attack_indicator_;
        attack_indicator_ = nullptr;
    }
}


void Thunder::DebugMenu()
{
    ImGui::Begin("Thunder");
    ImGui::Text("charge:%f", move_charge_);
    ImGui::Text("attack_charge:%f", attack_charge_);
    ImGui::SliderFloat("movePower", &movePower, 0.0f, 5.0f);
    ImGui::End();
}

ThunderAttack::ThunderAttack(Thunder* parent, Vector2 dir, float vel,float range): parent_(parent),range_(range),
                                                                       MovableObj(parent->GetPlayer()->GetPos(),
                                                                           atan2(-dir.y, dir.x),
                                                                           LoadTexture(Asset::GetAsset(fire_attack)),
                                                                           dir * vel),PlayerAttack(10000)
{
    start_pos_ = parent_->GetPlayer()->GetPos();
    SetPos(parent_->GetPlayer()->GetPos());
    SetScale(size_);
    SetType(OBJ_VOID);
}

void ThunderAttack::Update()
{
    std::list<Collider*> collisions = GetCollider()->GetCollision();
    for (auto collision : collisions)
    {
        OBJECT_TYPE type = collision->GetParent()->GetType();
        switch (type)
        {
        case OBJ_ENEMY:
            {
                Enemy* enemy = dynamic_cast<Enemy*>(collision->GetParent());
                if (enemy != nullptr)
                {
                    enemy->SetHp(enemy->GetHp() - GetDamage());
                }
            }
            break;
        default:
            break;
        }
    }
    if ((GetPos() - start_pos_).Distance() > range_)
        Discard();
    AddVel(GetVel());
}

ThunderIndicator::ThunderIndicator() : MovableObj(Vector2::Zero, 0.0f, LoadTexture(Asset::GetAsset(thunder_indicator)), Vector2::Zero)
{
    SetScale(Vector2(2 * GameObject::SIZE_, 0.5 * GameObject::SIZE_));
    SetType(OBJ_VOID);
}
