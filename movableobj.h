//--------------------------------------------------------------------------------
// 
// �����I�u�W�F�N�g[movableobj.h]
// 
// �쐬�� ���{��
// 
// �쐬��		2023/11/16
// �ŏI�X�V��	2023/11/17
// 
//--------------------------------------------------------------------------------

#pragma once

#include "lib/dxlib.h"

#include "gameobject.h"

class MovableObj : public GameObject
{
private:
	Vector2 vel_;

public:
	MovableObj(Vector2 pos,float rot,int tex_number,Vector2 vel)
		:GameObject(pos,rot,tex_number),vel_(vel) {}

	void SetVel(Vector2 vel) { vel_ = vel; }		//���x�Z�b�g
	Vector2 GetVel(void) const { return vel_; }		//���x�Q�b�g

	void AddVel(Vector2 vel) { SetPos(GetPos() + vel); }	//���x���I�u�W�F�N�g�ɑ���

	bool Collision(const GameObject* obj) const;	//�����蔻��

};
