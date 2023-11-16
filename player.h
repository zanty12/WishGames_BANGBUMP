//--------------------------------------------------------------------------------
// 
// プレイヤー[player.h]
// 
// 作成者 根本賢
// 
// 作成日		2023/11/16
// 最終更新日	2023/11/16
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
