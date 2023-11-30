//--------------------------------------------------------------------------------
// 
// 動くオブジェクト[movableobj.h]
// 
// 作成者 根本賢
// 
// 作成日		2023/11/16
// 最終更新日	2023/11/17
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

	void SetVel(Vector2 vel) { vel_ = vel; }		//速度セット
	Vector2 GetVel(void) const { return vel_; }		//速度ゲット

	void AddVel(Vector2 vel) { SetPos(GetPos() + vel); }	//速度をオブジェクトに足す

	bool Collision(const GameObject* obj) const;	//当たり判定

};
