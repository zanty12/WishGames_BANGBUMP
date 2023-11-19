//--------------------------------------------------------------------------------
// 
// �����I�u�W�F�N�g[movableobj.h]
// 
// �쐬�� ���{��
// 
// �쐬��		2023/11/16
// �ŏI�X�V��	2023/11/16
// 
//--------------------------------------------------------------------------------

#pragma once

#include "lib/dxlib.h"

#include "gameobject.h"

class MovableObj : public GameObject
{
private:
	Vector2 vel_;
	Vector2 pos_;

public:
	MovableObj(Vector2 pos,float rot,int tex_number,Vector2 vel)
		:GameObject(pos,rot,tex_number),vel_(vel) {}

	void SetVel(Vector2 vel) { vel_ = vel; }		//���x�Z�b�g
	Vector2 GetVel(void) const { return vel_; }		//���x�Q�b�g

	virtual void AddVel(Vector2 vel) = 0;			//���x���I�u�W�F�N�g�ɑ���

	void SetPos(Vector2 pos) { pos_ = pos; }		//�|�W�V�����Z�b�g
	Vector2 GetPos(void) const { return pos_; }		//�|�W�V�����Q�b�g

	virtual void AddPos(Vector2 pos) = 0;


};
