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

    const float responseMinStickDistance = 0.2f;    // �X�e�B�b�N�X�����Ƃ����肷�鋗��
    //const float maxChage = 100.0f;                  // �`���[�W�̍ő�l
    const float arrowGravity = 0.5f;                // ��̏d��
    const Vector2 arrowSize = Vector2(100, 10);     // ��̃T�C�Y��������
    //const float movePower = 1;                      // �ړ��̗�
    //const float attackPower = 5;                    // ��̈ړ��̗�
    float charge = 0.0f;                            // �`���[�W�̒l
    float responseMinOneFrameDistance = 0.2f;       // 1�t���[���ł̃X�e�B�b�N�ړ��ʁi�e���Ƃ��̔���l�j
    Arrow arrows_[3];

    //��������͒����p�̂���const����
    float maxCharge = 100.0f;
    float movePower = 1;
    float attackPower = 5;

public:
    Thunder(Player *player) : Attribute(player) {}
    ~Thunder() override = default;
    bool StickTrigger(Vector2 stick, Vector2 previousStick);
    Vector2 Move() override;
    void Action() override;
    void Draw(Vector2 offset) override;
    void DebugMenu() override;
};

