//--------------------------------------------------------------------------------
// 
// �T���_�[[thunder.h]
// 
// �쐬�� �⏁��
// 
// �ŏI�X�V��	2023/11/22
// 
//--------------------------------------------------------------------------------
#pragma once
#include "attribute.h"
#include "lib/list.h"

class ThunderAttack;
class Thunder : public Attribute
{
    struct Arrow {
        Vector2 position;
        Vector2 velocity;
        bool isVisible = false;
    };

    const float responseMinStickDistance = 0.5f;    // �X�e�B�b�N�X�����Ƃ����肷�鋗��
    //const float maxCharge = 100.0f;                  // �`���[�W�̍ő�l

    //const float movePower = 1;                      // �ړ��̗�
    float charge_ = 0.0f;                            // �`���[�W�̒l
    float responseMinOneFrameDistance = 0.2f;       // 1�t���[���ł̃X�e�B�b�N�ړ��ʁi�e���Ƃ��̔���l�j
    float move_cd_ = 1.0f;
    const float move_cd_max_ = 1.0f;
    Vector2 move_dir_;
    bool moving_ = false;
    //attack
    const float attack_charge_max_ = 400.0f;
    float attack_charge_ = 0.0f;
    float attack_cd_ = 0.0f;
    const float attack_vel_ = 10*GameObject::SIZE_;
    ThunderAttack* attack_[5] = {nullptr};

    //��������͒����p�̂���const����
    float maxCharge_ = 100.0f;
    float movePower = 11 * GameObject::SIZE_;

public:
    Thunder(Player* player) : Attribute(player, ATTRIBUTE_TYPE_THUNDER) {}
    ~Thunder()override ;
    bool StickTrigger(Vector2 stick, Vector2 previousStick);
    Vector2 Move() override;
    void Action() override;
    void DebugMenu() override;
};

class ThunderAttack : public MovableObj
{
    Thunder* parent_;
    Vector2 size_ = Vector2(2 * GameObject::SIZE_, 2 * GameObject::SIZE_);
    Vector2 start_pos_;
public:
    ThunderAttack() = delete;
    ThunderAttack(Thunder* parent,Vector2 dir,float vel);
    ~ThunderAttack() override = default;
    void Update() override;
};