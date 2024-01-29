#include "wind.h"

#include "asset.h"
#include "xinput.h"
#include "lib/collider2d.h"

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

    // 回転のスピードを取得
    float rotSpeed = Vector2::Cross(stick, previousStick);

    move_filter_.PassSignal(StickTrigger(stick, previousStick));
    int move = move_filter_.PredictNext();
    std::cout<<move<<std::endl;
    // 移動中
    if (move)
    {
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
    SetType(OBJ_VOID);
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
                    }
                }
            }
            break;
        default:
            break;
        }
    }
}