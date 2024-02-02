#pragma once
#include"attribute.h"
#include "playerattack.h"

class FireAttack;
class FireEffect;

class Fire : public Attribute
{
    FireAttack* attack_ = nullptr;
    FireEffect* move_effect_ = nullptr;

    Vector2 attackDirection;
    float brake_ = 0.50f;
    Vector2 vel_;



public:
    Fire(Player* player);
    ~Fire() override;
    bool StickTrigger(Vector2 stick, Vector2 previousStick = Vector2::Zero) override;
    void Move() override;
    void Action() override;
    void DebugMenu() override;
};

class FireAttack : public MovableObj,public PlayerAttack
{
    Fire* parent_;
    Vector2 size_ = Vector2(GameObject::SIZE_, 3 * GameObject::SIZE_);
public:
    FireAttack() = delete;
    FireAttack(Fire* parent);
    ~FireAttack() override = default;
    void Update() override;
};

class FireEffect : public MovableObj
{
private:
    Fire* parent_;

public:
    FireEffect() = delete;
    FireEffect(Fire* parent);
    ~FireEffect() override = default;
    void Update() override;
};
