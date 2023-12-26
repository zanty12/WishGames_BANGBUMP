//--------------------------------------------------------------------------------
// 
// プレイヤー[player.h]
// 
// 作成者 根本賢
// 
// 作成日		2023/11/16
// 最終更新日	2023/11/30
// 
//--------------------------------------------------------------------------------

#pragma once

#include "animator.h"
#include "movableobj.h"
#include "attribute.h"
#include "collidercir.h"
#include "mapmngr.h"

enum PLAYER_STATE
{
	MOVE_UP,		//上に移動
	FALL,			//落ちる
	TOUCH_GROUND,	//地面にいる
};

class MapMngr;
class Camera;
class SkillOrb;

class Player : public MovableObj
{
private:

	const int INITIAL_HP_ = 500;		//HPの初期値
	const float GRAVITY_SCALE_ = -6.0f;	//重力（仮）
	const int SPIKE_SURPRISE_ = 15;		//トゲに当たってノックバックするフレーム数

	Vector2 dir_;		//向き

	class Attribute* move_attribute_ = nullptr;		//動く用のアトリビュート
	class Attribute* attack_attribute_ = nullptr;	//攻撃用のアトリビュート

	int hp_;
	int skillpt_;
	int lv_;

	int drop_point_;		//落とすポイント
	ATTRIBUTE_TYPE hit_attack_attr = ATTRIBUTE_TYPE_FIRE;	//受けた攻撃属性

	bool change_scene_;		//シーン遷移フラグ

	MapMngr* map_mangr_;

	int clash_spike_;		//トゲに衝突したら15フレームの間ノックバック
	int knock_back_dir_;	//トゲに衝突した方向

	int not_stick_working_;

	PLAYER_STATE player_state_;

public:
	Player(Vector2 pos, float rot, int tex_number, Vector2 vel, MapMngr* map_mangr)
		:MovableObj(pos, rot, tex_number, vel), hp_(INITIAL_HP_), skillpt_(0), lv_(1),
		dir_(Vector2(0.0f, 0.0f)), map_mangr_(map_mangr), clash_spike_(0), knock_back_dir_(0),
		change_scene_(false), drop_point_(0)
	{}

	~Player() { delete move_attribute_; delete attack_attribute_; }

	void SetDir(Vector2 dir) { dir_ = dir; }	//向きのセット
	Vector2 GetDir(void) const { return dir_; }	//向きのゲット
	int GetHp(void) const { return hp_; }		//HPのゲット
	void SetHp(int hp) { hp_ = hp; }			//HPのセット
	void SetAttribute(Attribute* move_attribute) {delete move_attribute_; move_attribute_ = move_attribute; }				//ムーブアトリビュートポインタのセット（何も操作していないときはnullptrをセット）
	void SetAttackAttribute(Attribute* attack_attribute) {delete attack_attribute_; attack_attribute_ = attack_attribute; }	//アタックアトリビュートポインタのセット（何も操作していないときはnullptrをセット）
	Attribute* GetAttribute(void) const { return move_attribute_; }			//ムーブアトリビュートポインタをゲット（属性が何もなければnullptrを返す）
	Attribute* GetAttackAttribute(void) const { return attack_attribute_; }	//アタックアトリビュートポインタをゲット（属性が何もなければnullptrを返す）
	MapMngr* GetMapMngr(void) const { return map_mangr_; }	//MapMngrのポインタをゲット
	bool GetChangeSceneFlag(void) const { return change_scene_; }	//シーンチェンジのフラグ true=別のシーンへ
	PLAYER_STATE GetPlayerState(void) const { return player_state_; }	//プレイヤーのステータスをゲット
	int GetSkillPoint(void) const { return skillpt_; }	//現在所有しているスキルポイントのゲット

	//スキルポイントの減少（ダメージが所持スキルポイントを超える場合、スキルポイントは0になる）
	void SkillPointDown(int damage) { damage <= skillpt_ ? skillpt_ -= damage : skillpt_ = 0; }
	//HPの減少（ダメージが現在のHPを超える場合、HPは0になる）
	void HpDown(int damage) { damage <= hp_ ? hp_ -= damage : hp_ = 0; }

	SkillOrb* DropSkillOrb(void);

	void Update(void) override;
	void Draw(Camera* camera);

	void DebugMenu();

private:
	//向きのアップデート。速度をもとに更新（全く動いていない場合は止まった瞬間の向きのままにする）
	void UpdateDir(void) { if (GetVel() != Vector2(0.0f, 0.0f)) dir_ = GetVel().Normalize(); }

	//当たり判定
	void CollisionAction(void);
	//当たり判定（トゲ）
	void CollisionSpike(void);
	//当たり判定（スキルポイント）
	void CollisionSkillPoint(GameObject* obj);
	//当たり判定（アタックアトリビュート）
	void CollisionAttack(GameObject* obj);

	//レベルアップ（ゲットしたスキルポイントを引数にする）
	void LvUp(void);

	void HpMaxUp(void);
};
