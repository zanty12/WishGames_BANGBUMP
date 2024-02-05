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

Fire::~Fire()
{
    if (attack_)delete attack_;
    if (move_effect_)delete move_effect_;
}

bool Fire::StickTrigger(Vector2 stick, Vector2 previousStick)
{
    return stick.DistanceSq() > state_->minInputDistance * state_->minInputDistance;
}

void Fire::Move()
{
    Vector2 stick = Input::GetStickLeft(0);

    Vector2 stickR = Input::GetStickRight(0);
    if (abs(stick.x) < 0.01f && abs(stick.y) < 0.01f &&
        abs(stickR.x) < 0.01f && abs(stickR.y) < 0.01f)
    {
        if (player_->GetAnimator()->GetLoopAnim() != PLAYER_ATTACK_ANIM &&
            player_->GetAnimator()->GetLoopAnimNext() != PLAYER_ATTACK_ANIM)
            player_->GetAnimator()->SetLoopAnim(PLAYER_IDLE_ANIM);
    }

    //移動
    if (StickTrigger(stick))
    {
        //エフェクト表示
        move_effect_->SetColor(Color(1, 1, 1, 1));
        move_effect_->GetAnimator()->SetIsAnim(true);
        move_effect_->Update();
        //もう存在しない慣性移動
        /*Vector2 dir = stick.Normalize();
        Vector2 vel = player_->GetVel() + dir * speed * Time::GetDeltaTime() * Time::GetDeltaTime();
        Vector2 player_dir = player_->GetVel().Normalize();

        float angle = acos(Vector2::Dot(dir, player_dir));
        if (acos(Vector2::Dot(dir, player_dir)) > M_PI_2)
            vel += dir * speed * Time::GetDeltaTime() * Time::GetDeltaTime();*/
        AddPower(stick);
        ImGuiIO& io = ImGui::GetIO();
        float fps = io.Framerate;
        vel_ += CalcVector(stick / (fps / server_tick_rate));

        player_->SetGravityState(GRAVITY_NONE);

        //limit velocity
        float maxPowerSq = state_->maxPower * state_->maxPower;
        if (maxPowerSq < player_->GetVel().DistanceSq()) player_->SetVel( player_->GetVel().Normalize() * state_->maxPower);
    }

    //停止
    else
    {
        if (move_effect_) //操作をやめたら非表示
        {
            move_effect_->SetColor(Color(1, 1, 1, 0));
        }
        player_->SetGravityState(GRAVITY_FULL);

        Friction();
    }
    player_->SetVel(vel_);
    Friction();
};

void Fire::Action()
{
    using namespace PHYSICS;
    Vector2 stick = Input::GetStickRight(0);

    if (StickTrigger(stick))
    {
        player_->GetAnimator()->SetLoopAnim(PLAYER_ATTACK_ANIM);

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
}

FireAttack::FireAttack(Fire* parent) : parent_(parent),
                                       MovableObj(
                                           Vector2(
                                               parent->GetPlayer()->GetPos().x + parent->GetPlayer()->GetScale().x / 2 +
                                               1.5 * GameObject::SIZE_, parent->GetPlayer()->GetPos().y), 0.0f,
                                           LoadTexture(Asset::GetAsset(fire_attack)), Vector2::Zero),
                                       PlayerAttack(parent->GetState()->atk, parent->GetState()->atkCoolTime,
                                                    parent->GetState()->knockbackRate)
{
    SetScale(Vector2(parent->GetState()->atkDistance, parent->GetState()->atkRange));
    SetType(OBJ_ATTACK);

    //アニメーション設定
    GetAnimator()->SetTexenum(fire_attack);
    GetAnimator()->SetLoopAnim(FIRE_ATTACK_ANIM);
    GetAnimator()->SetDrawPriority(75);
}

void FireAttack::Update()
{
    std::list<Collider*> collisions = GetCollider()->GetCollision();
    for (auto collision : collisions)
    {
        OBJECT_TYPE type = collision->GetParent()->GetType();
        switch (type)
        {
        case OBJ_ENEMY:
            {
                //put collider into map if hit for first time
                target_[collision] = WIN::Time();
                target_[collision].Start();
            }
            break;
        default:
            break;
        }
    }

    //deal damage to all hit enemies
    for (auto& target : target_)
    {
        Enemy* enemy = dynamic_cast<Enemy*>(target.first->GetParent());
        if (target.second.GetNowTime() * 0.001f > tick_)
        {
            target.second.Start();
            enemy->SetHp(enemy->GetHp() - damage_);
            //エフェクトの生成
            Vector2 pos = target.first->GetParent()->GetPos();
            Vector2 scale = target.first->GetParent()->GetScale();
            AttachHitEffect(new AttackHitEffect(pos, scale, effect_hit_fire, EFFECT_HIT_FIRE_ANIM));
        }
    }
    HitEffectUpdate(); //エフェクトのアップデート
}

FireEffect::FireEffect(Fire* parent)
    : MovableObj(parent->GetPlayer()->GetPos(), 0.0f, LoadTexture(Asset::GetAsset(fire_move)), Vector2::Zero),
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

    parent_->GetPlayer()->GetAnimator()->SetLoopAnim(PLAYER_FD_MOVE_ANIM);
}
