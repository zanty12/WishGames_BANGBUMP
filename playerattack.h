#pragma once

class PlayerAttack
{
private:
    float damage_;
    float max_tick_ = 0.0f; //max time for each damage tick
    float tick_ = 0.0f; //time for each damage tick
public:
    PlayerAttack(float damage) : damage_(damage) {}
    virtual ~PlayerAttack() = default;
    void SetDamage(float damage) { damage_ = damage; }
    float GetDamage() const { return damage_; }
    void SetTick(float tick) { tick_ = tick; }
    float GetTick() const { return tick_; }
    void SetMaxTick(float max_tick) { max_tick_ = max_tick; }
    float GetMaxTick() const { return max_tick_; }
    void UpdateTick() { tick_ += Time::GetDeltaTime(); }
};
