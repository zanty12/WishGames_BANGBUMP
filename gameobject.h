//--------------------------------------------------------------------------------
// 
// �Q�[���I�u�W�F�N�g�i�C���^�[�t�F�[�X�j[gameobject.h]
// 
// �쐬�� ���{��
// 
// �쐬��		2023/11/16
// �ŏI�X�V��	2023/11/16
//
//  Update:
//  2023/11/16 [���{��] �Q�[���I�u�W�F�N�g�̃C���^�[�t�F�[�X���쐬
// 2023/11/30 Cheung To Hung �`���scale,color��ǉ�
//--------------------------------------------------------------------------------

#pragma once

#include "sprite.h"
#include "lib/dxlib.h"
#include "graphical.h"

enum OBJECT_TYPE
{
    OBJ_SOLID, //�ђʕs�\
    OBJ_PENETRABLE, //���ђ�
    OBJ_VOID, //�ђʉ\
    OBJ_SPIKE, //�g�Q
    OBJ_PLAYER, //�v���C���[
    OBJ_ENEMY, //�G
    OBJ_ATTACK, //�U��
};

class Animator;
class Collider;
class GameObject
{
private:
    Vector2 pos_;
    float rot_;
    int tex_;
    Vector2 scale_ = Vector2{SIZE_, SIZE_}; //�傫��
    Color color_ = Color(1.0f, 1.0f, 1.0f, 1.0f);
    Animator* animator_;
    Collider* collider_;

public:
    static constexpr float SIZE_ = 64.0f; //�P�}�X�̕W���T�C�Y
	GameObject() = default;
    GameObject(Vector2 pos, float rot, int tex_number);
    virtual ~GameObject() = default;

    void SetPos(Vector2 pos) { pos_ = pos; } //�|�W�V�����Z�b�g
    Vector2 GetPos(void) const { return pos_; } //�|�W�V�����Q�b�g
    void SetRot(float rot) { rot_ = rot; } //��]�Z�b�g
    float GetRot(void) const { return rot_; } //��]�Q�b�g
    void SetTexNo(int tex_number) { tex_ = tex_number; } //�e�N�X�`���Z�b�g
    int GetTexNo(void) const { return tex_; } //�e�N�X�`���Q�b�g
    void SetScale(Vector2 scale) { scale_ = scale; } //�傫���Z�b�g
    Vector2 GetScale(void) const { return scale_; } //�傫���Q�b�g
    void SetColor(Color color) { color_ = color; } //�F�Z�b�g
    Color GetColor(void) const { return color_; } //�F�Q�b�g
    Animator* GetAnimator(void) const { return animator_; } //�A�j���[�^�[�Q�b�g
    Collider* GetCollider(void) const { return collider_; } //�R���C�_�[�Q�b�g


    virtual void Update(void) = 0;

};
