#include "wind.h"

#include "asset.h"
#include "xinput.h"
#include "lib/collider2d.h"

Wind::Wind(Player* player)
    : Attribute(player, ATTRIBUTE_TYPE_WIND),move_effect_(new WindEffect(this))
{
}

bool Wind::StickTrigger(Vector2 stick, Vector2 previousStick)
{
    //TODO: rewrite triggering to give a more stable output
    float stickDistance = stick.Distance();
    float preStickDistance = previousStick.Distance();

    if (rotInputJudgeMin < MATH::Abs(Vector2::Cross(stick * 10, previousStick * 10)) &&
        0.8f < stickDistance * preStickDistance)
    {
        return true;
    }
    return false;
}

Vector2 Wind::Move(void)
{
    Vector2 vel = player_->GetVel();
    Vector2 stick = Input::GetStickLeft(0);
    Vector2 previousStick = Input::GetPreviousStickLeft(0);

    Vector2 stickR = Input::GetStickRight(0);
    if (abs(stick.x) < 0.01f && abs(stick.y) < 0.01f &&
        abs(stickR.x) < 0.01f && abs(stickR.y) < 0.01f)
    {
        if (player_->GetAnimator()->GetLoopAnim() != PLAYER_ATTACK_ANIM &&
            player_->GetAnimator()->GetLoopAnimNext() != PLAYER_ATTACK_ANIM)
            player_->GetAnimator()->SetLoopAnim(PLAYER_IDOL_ANIM);
    }

    // 回転のスピードを取得
    float rotSpeed = Vector2::Cross(stick, previousStick);

    move_filter_.PassSignal(StickTrigger(stick, previousStick));
    int move = move_filter_.PredictNext();
    std::cout<<move<<std::endl;
    // 移動中
    if (move)
    {
        player_->GetAnimator()->SetLoopAnim(PLAYER_TW_MOVE_ANIM);

        power_ += rotSpeed * rotSpeed * rotInputFriction / Time::GetDeltaTime();
        if (maxPower_ < power_) power_ = maxPower_;

        vel = Vector2::Up * power_ * Time::GetDeltaTime();
        player_->SetGravityState(GRAVITY_NONE);
        previous_time_ = Time::GetCurrentTime();
    }
    // 落下処理
    else if (/*0 < Vector2::Dot(Vector2::Down, vel) || prev_y_ > vel.y*/!move)
    {
        power_ *= friction_;

        vel.x = stick.x * 6 * GameObject::SIZE_ * Time::GetDeltaTime();
        player_->SetGravityState(GRAVITY_HALF);
        //上移動の慣性残っている場合
        if (vel.y > 0)
            vel.y -= vel.y * 2 * Time::GetDeltaTime();
    }
    else if (stick.Distance() < rotInputJudgeMin)
    {
        power_ *= friction_;
        player_->SetGravityState(GRAVITY_HALF);
    }

    if (prev_power_ < 0.0001f && power_ > 0.0001f)
    {
        //エフェクト表示
        move_effect_->SetPos(GetPlayer()->GetPos());
        move_effect_->DrawEffect();
    }

    prev_power_ = power_;
    move_effect_->Update();

    prev_y_ = vel.y;
    return vel;
}

void Wind::Action(void)
{
    using namespace PHYSICS;
    Vector2 stick = Input::GetStickRight(0);
    Vector2 previousStick = Input::GetPreviousStickRight(0);


    // 回転のスピードを取得
    float rotSpeed = Vector2::Cross(stick, previousStick);
    std::cout<<StickTrigger(stick, previousStick);
    attack_filter_.PassSignal(StickTrigger(stick, previousStick));
    int attack = attack_filter_.PredictNext();
    std::cout<<attack<<std::endl;
    // 攻撃中
    if (attack)
    {
        player_->GetAnimator()->SetLoopAnim(PLAYER_ATTACK_ANIM);

        if (attack_ == nullptr)
            attack_ = new WindAttack(this);
        attack_->SetPos(GetPlayer()->GetPos());
    }
    else if (attack_ != nullptr)
    {
        delete attack_;
        attack_ = nullptr;
    }
}

void Wind::DebugMenu()
{
    ImGui::Begin("Wind");
    ImGui::Text("power: %.2f", power_);
    ImGui::SliderFloat("MaxPower", &maxPower_, 0.0f, 1000.0f);
    ImGui::SliderFloat("Friction", &friction_, 0.0f, 1.0f);
    ImGui::End();
}

WindAttack::WindAttack(Wind* parent) : parent_(parent), MovableObj(parent->GetPlayer()->GetPos(), 0.0f,
                                           LoadTexture(Asset::GetAsset(wind_attack)), Vector2::Zero),PlayerAttack(10000)
{
    SetScale(size_);
    SetType(OBJ_ATTACK);

    //アニメーション設定
    GetAnimator()->SetTexenum(wind_attack);
    GetAnimator()->SetLoopAnim(WIND_ATTACK_ANIM);
    GetAnimator()->SetDrawPriority(25);
}

void WindAttack::Update()
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

                        //エフェクトの生成
                        Vector2 pos = enemy->GetPos();
                        Vector2 scale = enemy->GetScale();
                        AttachHitEffect(new AttackHitEffect(pos, scale, effect_hit_wind, EFFECT_HIT_WIND_ANIM));
                    }
                }
            }
            break;
        default:
            break;
        }
    }

    HitEffectUpdate();  //エフェクトのアップデート
}


WindEffect::WindEffect(Wind* parent)
    :MovableObj(parent->GetPlayer()->GetPos(), 0.0f, LoadTexture(Asset::GetAsset(wind_move)), Vector2::Zero),
    parent_(parent)
{
    SetType(OBJ_VOID);

    //アニメーション設定
    SetScale(Vector2(SIZE_ * 2, SIZE_ * 2));
    GetAnimator()->SetTexenum(wind_move);
    GetAnimator()->SetLoopAnim(WIND_MOVE_ANIM);
    GetAnimator()->SetDrawPriority(75);
    SetColor(Color(0, 0, 0, 0));
}

void WindEffect::Update()
{
    if (draw_)
    {
        SetColor(Color(1, 1, 1, 1));
        GetAnimator()->SetColor(GetColor());
        GetAnimator()->SetIsAnim(true);
        GetAnimator()->SetTexenum(wind_move);
        GetAnimator()->SetLoopAnim(WIND_MOVE_ANIM);

        draw_time_ += Time::GetDeltaTime();
        if (draw_time_ > 1.0f)
        {
            draw_ = false;
            draw_time_ = 0.0f;
        }
    }
    else
    {
        SetColor(Color(1, 1, 1, 0));
        GetAnimator()->SetColor(GetColor());
        GetAnimator()->SetIsAnim(false);
        GetAnimator()->SetLoopAnim(ANIM_NONE);
    }
}
