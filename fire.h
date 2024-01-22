#pragma once
#include"attribute.h"
#include "playerattack.h"

class FireAttack;

class Fire : public Attribute
{
    //const float speed = 10.0f;						// スピード
    float friction = 0.88f; // 摩擦定数
    const float responseMinStickDistance = 0.2f; // スティックの傾けたときに判定する最小値
    FireAttack* attack_ = nullptr;

    Vector2 attackDirection;
    bool isDraw = false;

    //調整のためにconst抜き
    float speed = 100 * GameObject::SIZE_;


public:
    Fire(Player* player) : Attribute(player, ATTRIBUTE_TYPE_FIRE)
    {
    }

    ~Fire() override = default;
    Vector2 Move() override;
    void Action() override;
    void DebugMenu() override;
};

class FireAttack : public MovableObj,public PlayerAttack
{
    Fire* parent_;
    Vector2 size_ = Vector2(3 * GameObject::SIZE_, GameObject::SIZE_);
public:
    FireAttack() = delete;
    FireAttack(Fire* parent);
    ~FireAttack() override = default;
    void Update() override{};
};
