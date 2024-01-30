#include "fire.h"

#include <libavutil/mathematics.h>

#include "asset.h"
#include "xinput.h"
#include "sprite.h"
#include"lib/collider2d.h"

Fire::Fire(Player* player)
    : Attribute(player, ATTRIBUTE_TYPE_FIRE), move_effect_(new FireEffect(this))
{
}

Vector2 Fire::Move()
{
    Vector2 stick = Input::GetStickLeft(0);
    stick.y *= -1;
    if (stick.Distance() > responseMinStickDistance && player_->GetVel().Distance() < speed * Time::GetDeltaTime())
    {
        //エフェクト表示
        move_effect_->SetColor(Color(1, 1, 1, 1));
        move_effect_->GetAnimator()->SetIsAnim(true);
        move_effect_->Update();

        Vector2 dir = stick.Normalize();
        Vector2 vel = player_->GetVel() + dir * speed * Time::GetDeltaTime() * Time::GetDeltaTime();
        Vector2 player_dir = player_->GetVel().Normalize();

        float angle = acos(Vector2::Dot(dir, player_dir));
        if (acos(Vector2::Dot(dir, player_dir)) > M_PI_2)
            vel += dir * speed * Time::GetDeltaTime() * Time::GetDeltaTime();
        player_->SetGravityState(GRAVITY_NONE);
        return vel;
    }
    else if (stick.Distance() > responseMinStickDistance && player_->GetVel().Distance() >= speed * Time::GetDeltaTime())
    {
        //エフェクト表示
        move_effect_->SetColor(Color(1, 1, 1, 1));
        move_effect_->GetAnimator()->SetIsAnim(true);
        move_effect_->Update();

        Vector2 dir = stick.Normalize();
        Vector2 vel = dir * speed * Time::GetDeltaTime();
        player_->SetGravityState(GRAVITY_NONE);
        return vel;
    }
    else
    {
        if (move_effect_)   //操作をやめたら非表示
        {
            move_effect_->SetColor(Color(1, 1, 1, 0));
        }

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
        attack_->SetRot(angle + (3.14f / 2));
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
                                           LoadTexture(Asset::GetAsset(fire_attack)), Vector2::Zero),
                                       PlayerAttack(10000)
{
    SetScale(size_);
    SetType(OBJ_ATTACK);
    SetMaxTick(1.0f / 3.0f);
    SetDamage(50);

    //アニメーション設定
    GetAnimator()->SetTexenum(fire_attack);
    GetAnimator()->SetLoopAnim(FIRE_ATTACK_ANIM);
    GetAnimator()->SetDrawPriority(75);
}

void FireAttack::Update()
{
    UpdateTick();
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
                    if (GetTick() > GetMaxTick())
                    {
                        SetTick(0.0f);
                        enemy->SetHp(enemy->GetHp() - GetDamage());
                    }
                }
            }
            break;
        default:
            break;
        }
    }
}

FireEffect::FireEffect(Fire* parent)
    :MovableObj(parent->GetPlayer()->GetPos(), 0.0f, LoadTexture(Asset::GetAsset(fire_move)), Vector2::Zero),
    parent_(parent)
{
    SetType(OBJ_VOID);

    //アニメーション設定
    SetScale(Vector2(SIZE_ * 2, SIZE_ * 2));
    GetAnimator()->SetTexenum(fire_move);
    GetAnimator()->SetLoopAnim(FIRE_MOVE_ANIM);
    GetAnimator()->SetDrawPriority(25);
    SetColor(Color(0, 0, 0, 0));
}

void FireEffect::Update()
{
    Vector2 stick = Input::GetStickLeft(0);

    float rot = atan2(-stick.y, -stick.x);
    SetRot(rot);

    Vector2 pos = parent_->GetPlayer()->GetPos();
    SetPos(pos);
}
