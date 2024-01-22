#pragma once
#include"attribute.h"
#include "playerattack.h"

class FireAttack;

class Fire : public Attribute
{
    //const float speed = 10.0f;						// �X�s�[�h
    float friction = 0.88f; // ���C�萔
    const float responseMinStickDistance = 0.2f; // �X�e�B�b�N�̌X�����Ƃ��ɔ��肷��ŏ��l
    FireAttack* attack_ = nullptr;

    Vector2 attackDirection;
    bool isDraw = false;

    //�����̂��߂�const����
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
