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

class Thunder : public Attribute
{
    struct Arrow {
        Vector2 position;
        Vector2 velocity;
        bool isVisible = false;
    };

    const float responseMinStickDistance = 0.5f;    // �X�e�B�b�N�X�����Ƃ����肷�鋗��
    //const float maxCharge = 100.0f;                  // �`���[�W�̍ő�l
    const float arrowGravity = 0.5f;                // ��̏d��
    const Vector2 arrowSize = Vector2(100, 10);     // ��̃T�C�Y��������
    //const float movePower = 1;                      // �ړ��̗�
    //const float attackPower = 5;                    // ��̈ړ��̗�
    float charge_ = 0.0f;                            // �`���[�W�̒l
    float responseMinOneFrameDistance = 0.2f;       // 1�t���[���ł̃X�e�B�b�N�ړ��ʁi�e���Ƃ��̔���l�j
    Arrow arrows_[3];
    float move_cd_ = 1.0f;
    const float move_cd_max_ = 1.0f;
    Vector2 move_dir_;
    bool moving_ = false;

    //��������͒����p�̂���const����
    float maxCharge_ = 100.0f;
    float movePower = 11 * GameObject::SIZE_;
    float attackPower = 5;

public:
    Thunder(Player* player) : Attribute(player, ATTRIBUTE_TYPE_THUNDER) {}
    ~Thunder() override = default;
    bool StickTrigger(Vector2 stick, Vector2 previousStick);
    Vector2 Move() override;
    void Action() override;
    void Draw(Vector2 offset) override;
    void DebugMenu() override;
};

