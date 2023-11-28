//--------------------------------------------------------------------------------
// 
// プレイヤー[player.h]
// 
// 作成者 根本賢
// 
// 作成日		2023/11/16
// 最終更新日	2023/11/20
// 
//--------------------------------------------------------------------------------

#pragma once

#include "lib/dxlib.h"

#include "sprite.h"
#include "movableobj.h"
#include "attribute.h"

class Player : public MovableObj
{
private:
	const int SKILL_GAUGE_MAX_ = 10;	//所持スキルポイントの上限
	const int HP_MAX_ = 1000;			//HPの上限
	const float GRAVITY_SCALE_ = 6.0f;	//重力（仮）

	Vector2 dir_;		//向き
	Vector2 scale_;		//大きさ（未定）
	Color color_;

	class Attribute* attribute_ = nullptr;
	int hp_;
	int skillpt_;

public:
	Player(Vector2 pos,float rot, int tex_number,Vector2 vel)
		:MovableObj(pos,rot,tex_number,vel),hp_(HP_MAX_),skillpt_(0),
		dir_(Vector2(0.0f,0.0f)), scale_(Vector2(100.0f,100.0f/*未定とりあえず100*/)),color_(Color(1.0f, 1.0f, 1.0f, 1.0f)) {}

	void SetDir(Vector2 dir) { dir_ = dir; }	//向きのセット
	Vector2 GetDir(void) const { return dir_; }	//向きのゲット
	int GetHp(void) const { return hp_; }		//HPのゲット
	void SetAttribute(Attribute* attribute) { attribute_ = attribute; }	//アトリビュートポインタのセット（何も操作していないときはnullptrをセット）

	//スキルポイントの使用（使えるとき=true 使うとスキルポイントは0になる）
	bool UseSkillPoint(void);
	//スキルポイントの増加（ゲットしたポイントと所持スキルポイントの合計が所持スキルポイントの上限を超える場合、所持スキルポイントは10になる）
	void SkillPointUp(int point) { skillpt_ + point <= SKILL_GAUGE_MAX_ ? skillpt_ += point : skillpt_ = SKILL_GAUGE_MAX_; }
	//スキルポイントの減少（ダメージが所持スキルポイントを超える場合、スキルポイントは0になる）
	void SkillPointDown(int damage) { damage <= skillpt_ ? skillpt_ -= damage : skillpt_ = 0; }
	//HPの減少（ダメージが現在のHPを超える場合、HPは0になる）
	void HpDown(int damage) { damage <= hp_ ? hp_ -= damage : hp_ = 0; }

	void Update(void) override;
	void Draw(void) override { DrawSprite(GetTexNo(), GetPos(), GetRot(), scale_, color_); }

private:
	//向きのアップデート。速度をもとに更新（全く動いていない場合は止まった瞬間の向きのままにする）
	void UpdateDir(void) { if (GetVel() != Vector2(0.0f, 0.0f)) dir_ = GetVel().Normalize(); }

};
