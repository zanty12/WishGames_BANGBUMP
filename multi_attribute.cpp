#include "lib/math.h"
#include "multi_attribute.h"
#include "multiplay.h"
#include "lib/collider2d.h"
#include "xinput.h"
#include "time.h"






ServerAttribute *ServerAttribute::Create(ServerPlayer *player, ATTRIBUTE_TYPE type) {
    switch (type) {
    case ATTRIBUTE_TYPE_FIRE: return new ServerFire(player);
    case ATTRIBUTE_TYPE_WIND: return new ServerWind(player);
    }
    return nullptr;
}

ClientAttribute *ClientAttribute::Create(ClientPlayer*player, ATTRIBUTE_TYPE type) {
    switch (type) {
    case ATTRIBUTE_TYPE_FIRE: return new ClientFire(player);
    case ATTRIBUTE_TYPE_WIND: return new ClientWind(player);
    }
    return nullptr;
}

/*******************************************************
  Fire
********************************************************/
bool ServerFire::StickTrigger(Vector2 stick, Vector2 previousStick) {
    return false;
}

void ServerFire::Move(void) {
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
    velocity.x *= friction;
    velocity.y *= friction;
    player->velocity = velocity;
}
void ServerFire::Attack(void) {
    using namespace PHYSICS;
    Vector2 stick = Input::GetStickRight(0);

    // 攻撃
    if (stick.Distance() > judgeScale && player->animType != ANIMATION_TYPE_MOVE) {
        player->animType = ANIMATION_TYPE_ATTACK;

        // 角度を調べる
        float angle = atan2(stick.y, stick.x);
        // 攻撃オブジェクトの生成
        if (attack_ == nullptr)
            attack_ = player->map->GetAttacks()->Add<ServerFireAttack>(player);

        // 座標の指定
        Vector2 pos = Vector2(cos(angle), -sin(angle)) * (player->transform.scale.x / 2 + attack_->transform.scale.x / 2);
        pos += player->transform.position;
        attack_->transform.position = pos;
        attack_->transform.rotation = angle;
    }
    else {
        attack_->Destroy();
        attack_ = nullptr;
    }

    player->attackVelocity = stick;
}

void ClientFire::Move(void) {
    Vector2 direction = player->velocity;
    float localScale = 100;

    // 描画する
    float denominator = moveAnims.size();
    float numerator = denominator * 0.5f;
    for (Animator &anim : moveAnims) {
        anim.anim.Draw(anim.pos - MultiPlayClient::offset, anim.rot, anim.scl, Color(1.0f, 1.0f, 1.0f, 1.0f - numerator / denominator));
        numerator += 0.5f;
    }

    // 時間を計測する
    DWORD currentTime = timeGetTime();
    DWORD deltaTime = currentTime - startTime;

    // アニメーションを作成する
    if (25 < deltaTime) {
        // 計測をリセットする
        startTime = currentTime;

        // 炎の位置をずらす
        direction += direction.Normal() * MATH::Rand(-0.25f, 0.25f);

        // アニメーション生成
        float distance = 50.0f;
        Vector2 pos = player->transform.position + -direction.Normalize() * distance;
        float rot = atan2f(direction.y, -direction.x);
        Vector2 scl = Vector2::One * localScale;
        Color col = Color::White;
        moveAnims.push_front({ pos, rot, scl, moveAnim });

        // 要素が多いなら削除
        if (10 < moveAnims.size()) {
            moveAnims.pop_back();
        }
    }
}
void ClientFire::Attack(void) {
}





/*******************************************************
  Water
********************************************************/
bool ServerWater::StickTrigger(Vector2 stick, Vector2 previousStick) {
    return false;
}
void ServerWater::Move(void) {

}
void ServerWater::Attack(void) {

}

void ClientWater::Move(void) {

}
void ClientWater::Attack(void) {

}





/*******************************************************
  Thunder
********************************************************/
bool ServerThunder::StickTrigger(Vector2 stick, Vector2 previousStick) {
    return false;
}
void ServerThunder::Move(void) {

}
void ServerThunder::Attack(void) {

}

void ClientThunder::Move(void) {

}
void ClientThunder::Attack(void) {

}






/*******************************************************
  Wind
********************************************************/
bool ServerWind::StickTrigger(Vector2 stick, Vector2 previousStick)
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
void ServerWind::Move(void) {
    Vector2 &vel = player->velocity;
    Vector2 stick = Input::GetStickLeft(0);
    Vector2 previousStick = Input::GetPreviousStickLeft(0);

    // 回転のスピードを取得
    float rotSpeed = Vector2::Cross(stick, previousStick);

    // 移動中
    if (StickTrigger(stick, previousStick))
    {
        player->animType = ANIMATION_TYPE_MOVE;
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
void ServerWind::Attack(void) {
    using namespace PHYSICS;
    Vector2 stick = Input::GetStickRight(0);
    Vector2 previousStick = Input::GetPreviousStickRight(0);


    // 回転のスピードを取得
    float rotSpeed = Vector2::Cross(stick, previousStick);

    // 攻撃中
    if (StickTrigger(stick, previousStick))
    {
        player->animType = ANIMATION_TYPE_ATTACK;
        if (attack_ == nullptr)
            attack_ = player->map->GetAttacks()->Add<ServerWindAttack>(player);
        attack_->transform.position = player->transform.position;
    }
    else if (attack_ != nullptr)
    {
        attack_->Destroy();
        attack_ = nullptr;
    }

    player->attackVelocity = stick;
}

void ClientWind::Move(void) {
    float localScale = 200;


    Vector2 pos = player->transform.position;
    float rot = 0.0f;
    Vector2 scl = Vector2::One * localScale;
    Color col = Color::White;
    moveAnim.Draw(pos - MultiPlayClient::offset, rot, scl, col);
}
void ClientWind::Attack(void) {
    float localScale = 200;


    Vector2 pos = player->transform.position;
    float rot = 0.0f;
    Vector2 scl = Vector2::One * localScale;
    Color col = Color::White;
    attackAnim.Draw(pos - MultiPlayClient::offset, rot, scl, col);
}
