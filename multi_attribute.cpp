#include "multi_attribute.h"
#include "lib/math.h"
#include "lib/collider2d.h"
#include "xinput.h"
#include "time.h"


/*******************************************************
  Wind
********************************************************/
bool MultiWind::StickTrigger(Vector2 stick, Vector2 previousStick)
{
    float stickDistance = stick.Distance();
    float preStickDistance = previousStick.Distance();

    if (rotInputJudgeMin < MATH::Abs(Vector2::Cross(stick, previousStick)) &&
        0.8f < stickDistance * preStickDistance)
    {
        return true;
    }
    return false;
}

void MultiWind::Move(void) {
    Vector2 &vel = player->velocity;
    Vector2 stick = Input::GetStickLeft(0);
    Vector2 previousStick = Input::GetPreviousStickLeft(0);

    // 回転のスピードを取得
    float rotSpeed = Vector2::Cross(stick, previousStick);

    // 移動中
    if (StickTrigger(stick, previousStick))
    {
        power_ += rotSpeed * rotSpeed * rotInputFriction / Time::GetDeltaTime();
        if (maxPower_ < power_) power_ = maxPower_;

        vel = Vector2::Up * power_ * Time::GetDeltaTime();
        previous_time_ = Time::GetCurrentTime();
    }
    // 落下処理
    else if (0 < Vector2::Dot(Vector2::Down, vel) || prev_y_ > vel.y || Time::GetDeltaTime(previous_time_) > 0.04f)
    {
        power_ *= friction_;

        vel.x = stick.x * 6 * Time::GetDeltaTime();
        //上移動の慣性残っている場合
        if (vel.y > 0)
            vel.y -= vel.y * 2 * Time::GetDeltaTime();
    }
    else if (stick.Distance() < rotInputJudgeMin)
    {
        power_ *= friction_;
    }
    prev_y_ = vel.y;
}

void MultiWind::Attack(void) {
    using namespace PHYSICS;
    Vector2 stick = Input::GetStickRight(0);
    Vector2 previousStick = Input::GetPreviousStickRight(0);


    // 回転のスピードを取得
    float rotSpeed = Vector2::Cross(stick, previousStick);

    // 攻撃中
    if (StickTrigger(stick, previousStick))
    {
        if (attack_ == nullptr)
            player->map->GetAttacks()->Add<MultiWindAttack>(player);
        attack_->transform.position = player->transform.position;
    }
    else if (attack_ != nullptr)
    {
        attack_->Destroy();
        attack_ = nullptr;
    }
}




/*******************************************************
  Fire
********************************************************/
bool MultiFire::StickTrigger(Vector2 stick, Vector2 previousStick) {
    return false;
}

void MultiFire::Move(void) {
    Vector2 &vel = player->velocity;
    Vector2 stick = Input::GetStickLeft(0);

    // 移動中
    if (stick.Distance() > judgeScale) {
        player->animType = ANIMATION_TYPE_MOVE;

        velocity += stick;
        if (velocity.Distance() > maxSpeed) velocity = velocity.Normalize() * maxSpeed;
    }
    // 停止中
    else {
        player->animType = ANIMATION_TYPE_IDEL;
    }
    velocity *= friction;
    player->velocity = velocity;
}

void MultiFire::Attack(void) {
    using namespace PHYSICS;
    Vector2 stick = Input::GetStickRight(0);

    // 攻撃
    if (stick.Distance() > judgeScale && player->animType != ANIMATION_TYPE_MOVE) {
        player->animType = ANIMATION_TYPE_ATTACK;

        // 角度を調べる
        float angle = atan2(stick.y, stick.x);
        // 攻撃オブジェクトの生成
        if (attack_ == nullptr)
            attack_ = new MultiFireAttack(player);

        // 座標の指定
        Vector2 pos = Vector2(cos(angle), -sin(angle)) * (player->transform.scale.x / 2 + attack_->transform.scale.x / 2);
        pos += player->transform.position;
        attack_->transform.position = pos;
        attack_->transform.rotation = angle;
    }
    else {
        delete attack_;
        attack_ = nullptr;
    }
}
