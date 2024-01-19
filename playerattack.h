#pragma once

class PlayerAttack
{
private:
    float damage_;
public:
    PlayerAttack(float damage) : damage_(damage) {}
    virtual ~PlayerAttack() = default;
    void SetDamage(float damage) { damage_ = damage; }
    float GetDamage() const { return damage_; }
};
