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

class Thunder : public Attribute
{
    const float responseMinStickDistance = 0.2f;    // �X�e�B�b�N�X�����Ƃ����肷�鋗��
    const float maxChage = 100.0f;       // �`���[�W�̍ő�l
    float charge = 0.0f;                // �`���[�W�̒l
    float responseMinOneFrameDistance = 0.2f;       // 1�t���[���ł̃X�e�B�b�N�ړ��ʁi�e���Ƃ��̔���l�j


public:

    Thunder(Player *player) : Attribute(player) {}
    Vector2 Move() override;
    void Action() override;

};

