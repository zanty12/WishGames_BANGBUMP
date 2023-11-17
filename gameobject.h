//--------------------------------------------------------------------------------
// 
// ゲームオブジェクト（インターフェース）[gameobject.h]
// 
// 作成者 根本賢
// 
// 作成日		2023/11/16
// 最終更新日	2023/11/16
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

	void SetPos(Vector2 pos) { pos_ = pos; }				//ポジションセット
	Vector2 GetPos(void) const { return pos_; }				//ポジションゲット
	void SetRot(float rot) { rot_ = rot; }					//回転セット
	float GetRot(void) const { return rot_; }				//回転ゲット
	void SetTexNo(int tex_number) { tex_ = tex_number; }	//テクスチャセット
	int GetTexNo(void) const { return tex_; }				//テクスチャゲット

	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

};
