//--------------------------------------------------------------------------------
// 
// �Q�[���I�u�W�F�N�g�i�C���^�[�t�F�[�X�j[gameobject.h]
// 
// �쐬�� ���{��
// 
// �쐬��		2023/11/16
// �ŏI�X�V��	2023/11/16
// 
//--------------------------------------------------------------------------------

#pragma once

#include "lib/dxlib.h"

class GameObject
{
private:
	Vector2 pos_;
	float rot_;
	int tex_;

public:
	GameObject(Vector2 pos,float rot,int tex_number)
		:pos_(pos),rot_(rot),tex_(tex_number) {}

	void SetPos(Vector2 pos) { pos_ = pos; }				//�|�W�V�����Z�b�g
	Vector2 GetPos(void) const { return pos_; }				//�|�W�V�����Q�b�g
	void SetRot(float rot) { rot_ = rot; }					//��]�Z�b�g
	float GetRot(void) const { return rot_; }				//��]�Q�b�g
	void SetTexNo(int tex_number) { tex_ = tex_number; }	//�e�N�X�`���Z�b�g
	int GetTexNo(void) const { return tex_; }				//�e�N�X�`���Q�b�g

	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

};
