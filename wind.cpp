#include "wind.h"
#include "xinput.h"
#include "lib/collider2d.h"

bool Wind::StickTrigger(Vector2 stick, Vector2 previousStick) {
	float stickDistance = stick.Distance();
	float preStickDistance = previousStick.Distance();

	if (rotInputJudgeMin < MATH::Abs(Vector2::Cross(stick, previousStick)) &&
		0.8f < stickDistance * preStickDistance) {
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

    // 移動中
    if (StickTrigger(stick, previousStick))
    {
        power_ += rotSpeed * rotSpeed * rotInputFriction / Time::GetDeltaTime();
        if (maxPower_ < power_) power_ = maxPower_;

        vel = Vector2::Up * power_ * Time::GetDeltaTime();
        player_->SetGravityState(GRAVITY_NONE);
        previous_time_ = Time::GetCurrentTime();
    }
    // 落下処理
    else if (0 < Vector2::Dot(Vector2::Down, vel) || prev_y_ > vel.y || Time::GetDeltaTime(previous_time_) > 0.04f)
    {
        power_ *= friction_;

        vel.x = stick.x * 6 * GameObject::SIZE_ * Time::GetDeltaTime();
        player_->SetGravityState(GRAVITY_HALF);
        //上移動の慣性残っている場合
        if(vel.y > 0)
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
    isDraw = false;

    // 回転のスピードを取得
    float rotSpeed = Vector2::Cross(stick, previousStick);

    // 攻撃中
    if (StickTrigger(stick, previousStick))
    {
        attackCollider = Vertex1(player_->GetPos(), attackRadius);

        isDraw = true;
        auto enemies = player_->GetMapMngr()->GetEnemyMngr()->GetEnemies();

        for (auto enemy : enemies)
        {
            if (enemy)
            {
                Vertex4 enemyCollider(enemy->GetPos(), 0.0f, enemy->GetScale());

                if (Collider2D::Touch(attackCollider, enemyCollider))
                {
                    enemy->Discard();
                }
            }
        }
    }
}

void Wind::Draw(Vector2 offset)
{
    if (isDraw) DrawCollider(attackCollider, Color::Green, offset);
}

void Wind::DebugMenu()
{
    ImGui::Begin("Wind");
    ImGui::Text("power: %.2f", power_);
    ImGui::SliderFloat("MaxPower", &maxPower_, 0.0f, 1000.0f);
    ImGui::SliderFloat("Friction", &friction_, 0.0f, 1.0f);
    ImGui::SliderFloat("AttackRadius", &attackRadius, 0.0f, 100.0f);
    ImGui::End();
}

void Wind::Gravity()
{
}
