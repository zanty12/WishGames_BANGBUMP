//--------------------------------------------------------------------------------
// 
// �v���C���[[player.h]
// 
// �쐬�� ���{��
// 
// �쐬��		2023/11/16
// �ŏI�X�V��	2023/11/16
// 
//--------------------------------------------------------------------------------

#pragma once

#include "lib/dxlib.h"

#include "movableobj.h"

class Player : public MovableObj
{
private:

public:
	Player(Vector2 pos,float rot, int tex_number,Vector2 vel)
		:MovableObj(pos,rot,tex_number,vel) {}

	void Update(void) override {}
	void Draw(void) override {}
	void AddVel(Vector2 vel) override {}
};
