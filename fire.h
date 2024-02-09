#pragma once
#include"attribute.h"
#include "playerattack.h"

class FireAttack;
class FireEffect;

class Fire : public Attribute
{
    //const float speed = 10.0f;						// スピード
    float friction = 0.88f; // 摩擦定数
    const float responseMinStickDistance = 0.2f; // スティックの傾けたときに判定する最小値
    FireAttack* attack_ = nullptr;
    FireEffect* move_effect_ = nullptr;

    Vector2 attackDirection;
    bool isDraw = false;

    //調整のためにconst抜き
    float speed = 10 * GameObject::SIZE_;


public:
    Fire(Player* player);

    ~Fire() override { 
        if (attack_)delete attack_;
        if (move_effect_)delete move_effect_;
    }
    Vector2 Move() override;
    void Action() override;
    void DebugMenu() override;
    void Gatchanko(bool is_attack) override;
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
