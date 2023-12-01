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

class GameObject
{
private:
    Vector2 pos_;
    float rot_;
    int tex_;
    Vector2 scale_ = Vector2{size_, size_}; //�傫��
    Color color_ = Color(1.0f, 1.0f, 1.0f, 1.0f);

public:
    static constexpr float size_ = 96.0f; //�P�}�X�̕W���T�C�Y\
	GameObject() = default;
    GameObject(Vector2 pos, float rot, int tex_number)
        : pos_(pos), rot_(rot), tex_(tex_number)
    {
    }

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


    virtual void Update(void) = 0;

    virtual void Draw(void)
    {
        const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
        const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
        DrawSprite(GetTexNo(), Vector2(GetPos().x * scale_x, GetPos().y * scale_y), GetRot(),
                   Vector2(scale_.x * scale_x, scale_.y * scale_y), color_);
    }

    virtual void Draw(Vector2 offset)
    {
        const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
        const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
        DrawSprite(GetTexNo(), Vector2((GetPos().x - offset.x) * scale_x, (GetPos().y - offset.y) * scale_y), GetRot(),
                   Vector2(scale_.x * scale_x, scale_.y * scale_y), color_);
    }
};
